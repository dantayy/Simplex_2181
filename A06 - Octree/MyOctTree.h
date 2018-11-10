#pragma once

#include "MyOctant.h"

namespace Simplex 
{
	class MyOctTree
	{
		MyOctant root; //root of the Octree
		MyEntityManager manager; //entity manager to refer to when trying to locate objects within octants within this tree

	public:
		MyOctTree();
		~MyOctTree();

		//traversal method
		//insertion method
		//add objects method
	};
}

