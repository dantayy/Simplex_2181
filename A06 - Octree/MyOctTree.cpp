#include "MyOctTree.h"

using namespace Simplex;

MyOctTree::MyOctTree()
{
}

Simplex::MyOctTree::MyOctTree(MyOctant * sourceRoot, MyEntityManager * sourceManager)
{
	root = sourceRoot;
	manager = sourceManager;
}


MyOctTree::~MyOctTree()
{
}



void Simplex::MyOctTree::Insert(MyOctant* node)
{
	//node to possibly add children to
	MyOctant* current = node;

	//recursively try to insert for all nodes further down the tree
	if (current->hasChildren)
	{
		for (MyOctant* child : current->children)
		{
			Insert(child);
		}
	}

	//only check for insertion on nodes that don't have children
	else
	{
		//check to see if child nodes need to be made for this node given how many objects are currently in it
		if (current->numEntities > idealEntityNum)
		{
			//create the children
			current->children[0] = new MyOctant(current->minPoint, current->centerPoint);
			current->children[1] = new MyOctant(vector3(current->centerPoint.x, current->minPoint.y, current->minPoint.z),vector3(current->maxPoint.x, current->centerPoint.y, current->centerPoint.z));
			current->children[2] = new MyOctant(vector3(current->minPoint.x, current->minPoint.y, current->centerPoint.z), vector3(current->centerPoint.x, current->centerPoint.y, current->maxPoint.z));
			current->children[3] = new MyOctant(vector3(current->centerPoint.x, current->minPoint.y, current->centerPoint.z), vector3(current->maxPoint.x, current->centerPoint.y, current->maxPoint.z));
			current->children[4] = new MyOctant(vector3(current->minPoint.x, current->centerPoint.y, current->minPoint.z), vector3(current->centerPoint.x, current->maxPoint.y, current->centerPoint.z));
			current->children[5] = new MyOctant(vector3(current->centerPoint.x, current->centerPoint.y, current->minPoint.z), vector3(current->maxPoint.x, current->maxPoint.y, current->centerPoint.z));
			current->children[6] = new MyOctant(vector3(current->minPoint.x, current->centerPoint.y, current->centerPoint.z), vector3(current->centerPoint.x, current->maxPoint.y, current->maxPoint.z));
			current->children[7] = new MyOctant(current->centerPoint, current->maxPoint);

			//set the hasChildren bool
			current->hasChildren = true;

			//reset current's object array now that the children will hold the objects
			current->m_mEntityArray = nullptr;
			current->numEntities = 0;
		}
	}
}

void Simplex::MyOctTree::Delete(MyOctant * node)
{
	//node to possibly delete the children of
	MyOctant* current = node;

	//check for children, if none then don't do anything
	if (current->hasChildren)
	{
		//check all children to see if this current node has grandchildren
		bool hasGrandchildren = false;
		for (MyOctant* child : current->children)
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
			for (MyOctant* child : current->children)
			{
				Delete(child);
			}
		}
		else
		{
			//delete the children since none of them have children
			for (MyOctant* child : current->children)
			{
				SafeDelete(child);
			}
			current->hasChildren = false;
		}
	}
}

void Simplex::MyOctTree::AddObjs(MyOctant* node)
{
	//node to possibly add objects to
	MyOctant* current = node;

	//recursively add objects to all nodes further down the tree
	if (current->hasChildren)
	{
		for (MyOctant* child : current->children)
		{
			AddObjs(child);
		}
	}

	//only add entities/objects to leaf nodes
	else
	{
		//check corners of each object from the entity manager against the space of the leaf node
		for (size_t i = 0; i < manager->GetEntityCount(); i++)
		{
			MyEntity* object = manager->GetEntity(i);
			//assign the corners to be checked (can just use the global mins/maxs since none of the objs will be rotated)
			vector3 corners[8] = {};
			corners[0] = object->GetRigidBody()->GetMinGlobal();
			corners[1] = vector3(object->GetRigidBody()->GetMinGlobal().x, object->GetRigidBody()->GetMaxGlobal().y, object->GetRigidBody()->GetMinGlobal().z);
			corners[2] = vector3(object->GetRigidBody()->GetMinGlobal().x, object->GetRigidBody()->GetMinGlobal().y, object->GetRigidBody()->GetMaxGlobal().z);
			corners[3] = vector3(object->GetRigidBody()->GetMinGlobal().x, object->GetRigidBody()->GetMaxGlobal().y, object->GetRigidBody()->GetMaxGlobal().z);
			corners[4] = vector3(object->GetRigidBody()->GetMaxGlobal().x, object->GetRigidBody()->GetMinGlobal().y, object->GetRigidBody()->GetMinGlobal().z);
			corners[5] = vector3(object->GetRigidBody()->GetMaxGlobal().x, object->GetRigidBody()->GetMaxGlobal().y, object->GetRigidBody()->GetMinGlobal().z);
			corners[6] = vector3(object->GetRigidBody()->GetMaxGlobal().x, object->GetRigidBody()->GetMinGlobal().y, object->GetRigidBody()->GetMaxGlobal().z);
			corners[7] = object->GetRigidBody()->GetMaxGlobal();

			//if a corner falls completely inside the leaf, then add the object to the leaf's array of entities and break so it doesn't get added multiple times
			for (vector3 corner : corners)
			{
				if (corner.x > current->minPoint.x &&
					corner.y > current->minPoint.y &&
					corner.z > current->minPoint.z &&
					corner.x < current->maxPoint.x &&
					corner.y < current->maxPoint.y &&
					corner.z < current->maxPoint.z)
				{
					current->AddEntity(object);
					break;
				}
			}
		}
	}
}

void Simplex::MyOctTree::CollisionInLeaf(MyOctant * node)
{
	//node to analyze for shared entities
	MyOctant* current = node;

	//recursively check children (if there are any) and see if the entities are shared within them
	if (current->hasChildren)
	{
		for (MyOctant* child : current->children)
		{
			CollisionInLeaf(child);
		}
	}

	current->IsColliding();
}
