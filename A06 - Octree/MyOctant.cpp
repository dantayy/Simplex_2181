#include "MyOctant.h"

using namespace Simplex;

MyOctant::MyOctant()
{
}

Simplex::MyOctant::MyOctant(vector3 minP, vector3 maxP)
{
	minPoint = minP;
	maxPoint = maxP;
	float halfWidth = std::abs(maxP.x - minP.x);
	float halfLength = std::abs(maxP.z - minP.z);
	float halfHeight = std::abs(maxP.y - minP.y);
	centerPoint = vector3(minPoint.x + halfWidth, minPoint.y + halfHeight, minPoint.z + halfLength);
	MeshManager::GetInstance()->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, centerPoint) * glm::scale(vector3(halfWidth, 0.0f, 0.0f) * 2.0f), C_YELLOW);
}


MyOctant::~MyOctant()
{
}

void Simplex::MyOctant::AddEntity(MyEntity * entity)
{
	//Create a temporal entity to store the object
	MyEntity* pTemp = entity;
	//if I was able to generate it add it to the list
	if (pTemp->IsInitialized())
	{
		//create a new temp array with one extra entry
		PEntity* tempArray = new PEntity[numEntities + 1];
		//start from 0 to the current count
		uint uCount = 0;
		for (uint i = 0; i < numEntities; ++i)
		{
			tempArray[uCount] = m_mEntityArray[i];
			++uCount;
		}
		tempArray[uCount] = pTemp;
		//if there was an older array delete
		if (m_mEntityArray)
		{
			delete[] m_mEntityArray;
		}
		//make the member pointer the temp pointer
		m_mEntityArray = tempArray;
		//add one entity to the count
		++numEntities;
	}
}

bool Simplex::MyOctant::IsColliding(MyEntity * ent)
{
	//get their IDs to check
	String ent1ID = ent->GetUniqueID();
	//check all the entities within this octant against the one passed
	for (size_t i = 0; i < numEntities; i++)
	{
		if (ent->IsColliding(m_mEntityArray[i]))
		{
			return true;
		}
	}

	return false;
}
