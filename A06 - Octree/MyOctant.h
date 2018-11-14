#pragma once

#include "MyEntityManager.h"

namespace Simplex
{
	class MyOctant
	{

	public:

		typedef MyEntity* PEntity; //MyEntity Pointer
		PEntity* m_mEntityArray = nullptr; //array of MyEntity pointers
		uint numEntities = 0; //number of entities within this node specifically

		vector3 centerPoint = vector3(); //center point of the octant
		vector3 minPoint = vector3(); //center point of the octant
		vector3 maxPoint = vector3(); //center point of the octant

		typedef MyOctant* OctantPointer; //MyOctant Pointer
		OctantPointer children[8] = {}; //array of child octant pointers
		bool hasChildren = false; //bool denoting whether or not a node has children

		MyOctant();
		MyOctant(vector3 minP, vector3 maxP);
		~MyOctant();

		void AddEntity(MyEntity* entity);

		bool IsColliding(MyEntity* ent); //check if the two passed entities exist in this octant
	};
}
