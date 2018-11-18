#include "MyOctant.h"
using namespace Simplex;
uint MyOctant::m_nCount = 0;
//  MyOctant
void MyOctant::Init(void)
{
	m_nData = 0;
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}
}

MyOctant::MyOctant()
{
	Init();
	typedef MyEntity* PEntity; //MyEntity Pointer
	PEntity* m_mEntityArray = m_pEntityMngr->m_mEntityArray; //array of MyEntity pointers
	uint m_uEntityCount = m_pEntityMngr->m_uEntityCount; //number of elements in the array
	std::vector<vector3> v3MaxMin_list;
	for (uint i = 0; i < m_uEntityCount; ++i)
	{
		MyRigidBody* pRG = m_mEntityArray[i]->GetRigidBody();
		vector3 v3Min = pRG->GetMinGlobal();
		vector3 v3Max = pRG->GetMaxGlobal();
		v3MaxMin_list.push_back(v3Min);
		v3MaxMin_list.push_back(v3Max);
	}

	m_pRigidBody = new MyRigidBody(v3MaxMin_list);
	m_pRigidBody->MakeCubic();
	m_iID = m_nCount;
	//determine whether or not this octant has the ideal # of entities within it while also setting the dimension for the entities within this octant to this octant's unique ID
	idealAchieved = IsColliding();
}

MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	Init();
	std::vector<vector3> v3MaxMin_list;
	v3MaxMin_list.push_back(a_v3Center - vector3(a_fSize));
	v3MaxMin_list.push_back(a_v3Center + vector3(a_fSize));
	m_pRigidBody = new MyRigidBody(v3MaxMin_list);
	m_nCount++;
	m_iID = m_nCount;
	//determine whether or not this octant has the ideal # of entities within it while also setting the dimension for the entities within this octant to this octant's unique ID
	idealAchieved = IsColliding();
}

void MyOctant::Subdivide()
{
	//if this octant has achieved the ideal number of entities, do not perform this action
	if (idealAchieved) return;

	//if this octant has children, recursively subdivide the children instead
	if (hasChildren)
	{
		for (MyOctant* child : m_pChild)
		{
			child->Subdivide();
		}
	}
	else
	{
		vector3 v3Center = m_pRigidBody->GetCenterLocal();
		vector3 v3HalfWidth = m_pRigidBody->GetHalfWidth();
		float fSize = (v3HalfWidth.x) / 2.0f;
		float fCenters = fSize;

		//these need to be safedeleted at some point
		m_pChild[0] = new MyOctant(v3Center + vector3(fCenters, fCenters, fCenters), fSize);
		m_pChild[1] = new MyOctant(v3Center + vector3(-fCenters, fCenters, fCenters), fSize);
		m_pChild[2] = new MyOctant(v3Center + vector3(-fCenters, -fCenters, fCenters), fSize);
		m_pChild[3] = new MyOctant(v3Center + vector3(fCenters, -fCenters, fCenters), fSize);

		m_pChild[4] = new MyOctant(v3Center + vector3(fCenters, fCenters, -fCenters), fSize);
		m_pChild[5] = new MyOctant(v3Center + vector3(-fCenters, fCenters, -fCenters), fSize);
		m_pChild[6] = new MyOctant(v3Center + vector3(-fCenters, -fCenters, -fCenters), fSize);
		m_pChild[7] = new MyOctant(v3Center + vector3(fCenters, -fCenters, -fCenters), fSize);

		for (uint i = 0; i < 8; i++)
		{
			m_pChild[i]->m_nLevel = m_nLevel + 1;
			m_pChild[i]->m_pParent = this;
		}
		hasChildren = true;
	}
}

void Simplex::MyOctant::Undivide(void)
{
	//check for children, if none then don't do anything
	if (hasChildren)
	{
		//check all children to see if this current node has grandchildren
		bool hasGrandchildren = false;
		for (MyOctant* child : m_pChild)
		{
			if (child->hasChildren)
			{
				hasGrandchildren = true;
				break;
			}
		}
		//if the current node has grandchildren, just recurse into each child
		if (hasGrandchildren)
		{
			for (MyOctant* child : m_pChild)
			{
				child->Undivide();
			}
		}
		else
		{
			//delete the children since none of them have children
			for (uint i = 0; i < 8; i++)
			{
				//go through the entity array and check for collisions against this octant's rigid body to remove
				for (uint j = 0; j < m_pEntityMngr->GetEntityCount(); ++j)
				{
					//the entity existed within this octant, remove it from the entity's array of octants
					if (m_pEntityMngr->m_mEntityArray[j]->IsInDimension(m_pChild[i]->m_iID))
					{
						m_pEntityMngr->m_mEntityArray[j]->RemoveDimension(m_pChild[i]->m_iID);
					}
				}
				SafeDelete(m_pChild[i]);
				m_nCount--;
			}
			hasChildren = false;
			//reset entities within this now undivided octant
			idealAchieved = IsColliding();
		}
	}
}

void MyOctant::Swap(MyOctant& other)
{
	std::swap(m_nData, other.m_nData);
	std::swap(m_lData, other.m_lData);
}
void MyOctant::Release(void)
{
	m_lData.clear();
	SafeDelete(m_pRigidBody);
}
void Simplex::MyOctant::Display(void)
{
	m_pRigidBody->AddToRenderList();
	for (uint i = 0; i < 8; i++)
	{
		if (m_pChild[i])
			m_pChild[i]->Display();
	}
}
bool Simplex::MyOctant::IsColliding(void)
{
	typedef MyEntity* PEntity; //MyEntity Pointer
	PEntity* m_mEntityArray = m_pEntityMngr->m_mEntityArray; //array of MyEntity pointers
	uint m_uEntityCount = m_pEntityMngr->m_uEntityCount; //number of elements in the array

	//if this octant has children, recursively check for collision with the children instead
	if (hasChildren)
	{
		//reset the dimension arrays for all the entities so that when they're checked for collision again they only have the leaf nodes in their arrays
		for (uint i = 0; i < m_uEntityCount; ++i)
		{
			m_mEntityArray[i]->ClearDimensionSet();
		}
		for (MyOctant* child : m_pChild)
		{
			child->Subdivide();
		}
		return false; //this dimension clearly did not have the ideal number of entities since it has subdivisions
	}
	else
	{
		uint numCollisions = 0; //number of octant/entity collisions
		//go through the entity array and check for collisions against this octant's rigid body
		for (uint i = 0; i < m_uEntityCount; ++i)
		{
			MyRigidBody* pRB = m_mEntityArray[i]->GetRigidBody();
			//the entity exists within this octant, add it to the entities array of octants it exists in
			if (pRB->IsColliding(m_pRigidBody))
			{
				m_mEntityArray[i]->AddDimension(m_iID);
				numCollisions++;
			}
		}

		//return true if the # of entities within this octant is less than or equal to the ideal
		if (numCollisions <= idealNumEntities)
			return true;
		return false;
	}
}
MyOctant::MyOctant(MyOctant const& other)
{
	m_nData = other.m_nData;
	m_lData = other.m_lData;
}
MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}
MyOctant::~MyOctant() { Release(); };
//Accessors
void MyOctant::SetData(int a_nData) { m_nData = a_nData; }
int MyOctant::GetData(void) { return m_nData; }
void MyOctant::SetDataOnVector(int a_nData) { m_lData.push_back(a_nData); }
int& MyOctant::GetDataOnVector(int a_nIndex)
{
	int nIndex = static_cast<int>(m_lData.size());
	assert(a_nIndex >= 0 && a_nIndex < nIndex);
	return m_lData[a_nIndex];
}
//--- Non Standard Singleton Methods

