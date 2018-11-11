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

bool Simplex::MyOctant::ShareOctant(MyEntity * ent1, MyEntity * ent2)
{
	//get their IDs to check
	String ent1ID = ent1->GetUniqueID();
	String ent2ID = ent2->GetUniqueID();
	//check all the entities within this octant to see if the two passed entities both exist within this one
	bool ent1InThisLeaf = false;
	bool ent2InThisLeaf = false;
	for (size_t i = 0; i < numEntities; i++)
	{
		if (m_mEntityArray[i]->GenUniqueID == ent1ID)
		{
			ent1InThisLeaf = true;
		}
		else if (m_mEntityArray[i]->GenUniqueID == ent2ID)
		{
			ent2InThisLeaf = true;
		}
	}

	//if they both exist within this octant, return true
	if (ent1InThisLeaf && ent2InThisLeaf) return true;

	return false;
}
