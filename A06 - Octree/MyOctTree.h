#pragma once

#include "MyOctant.h"

namespace Simplex 
{
	class MyOctTree
	{

	public:
		MyOctant* root; //root of the Octree
		MyEntityManager* manager; //entity manager to refer to when trying to locate objects within octants within this tree
		uint idealEntityNum = 5; //ideal number of entities to be had within a leaf

		//default constructor
		MyOctTree();
		//parameterized constructor
		MyOctTree(MyOctant* sourceRoot, MyEntityManager* sourceManager);
		//destructor
		~MyOctTree();

		//insertion method
		void Insert(MyOctant* node);
		//deletion method (deleting leaves from lowest level of tree)
		void Delete(MyOctant* node);
		//add objects method
		void AddObjs(MyOctant* node);
		//check for shared octancy method
		void CollisionInLeaf(MyOctant* node);
	};
}

