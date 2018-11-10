#pragma once

#include "MyEntityManager.h"

namespace Simplex
{
	class MyOctant
	{

	public:

		typedef MyEntity* PEntity; //MyEntity Pointer
		PEntity* m_mEntityArray = nullptr; //array of MyEntity pointers

		vector3 centerPoint = vector3(); //center point of the octant
		vector3 minPoint = vector3(); //center point of the octant
		vector3 maxPoint = vector3(); //center point of the octant

		typedef MyOctant* OctantPointer; //MyOctant Pointer
		OctantPointer children[8] = {}; //array of child octant pointers

		MyOctant();
		~MyOctant();

	};
}
