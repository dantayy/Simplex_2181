/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2017/06
----------------------------------------------*/
#ifndef __MYOCTANT_H_
#define __MYOCTANT_H_

#include "Simplex\Simplex.h"
#include "MyEntityManager.h"

namespace Simplex
{

	//System Class
	class MyOctant
	{
		int m_nData = 0; //Number of elements in the list of elements
		std::vector<int> m_lData; //list of elements
		MeshManager* m_pMeshMngr = nullptr; //Pointer to Mesh manager
		MyEntityManager* m_pEntityMngr = nullptr; //Pointer to the Entity manager
		MyRigidBody* m_pRigidBody = nullptr; //rigid body for this octant
		uint m_iID = 0; //unique ID of the octant
		static uint m_nCount; //total number of octants
		MyOctant* m_pParent = nullptr; //parent of this octant
		MyOctant* m_pChild[8]; //children of this octant
		bool hasChildren = false; //bool determining whether or not this octant has children
		uint m_nLevel = 0; //level this octant is at
		uint idealNumEntities = 5; //ideal number of entities to exist within this octant
		bool idealAchieved = false; //bool determining whether or not this octant has the ideal number of entities or less

	public:
		bool isVisible = true; //bool for determining if the wireframes of the octant should be shown

		/*
		USAGE: Displays the octant's rigid body
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Display(void);
		/*
		USAGE: Finds how many entities are "colliding" with the octant and sets their dimension accordingly
		ARGUMENTS: ---
		OUTPUT: whether or not the octant has the ideal number of entities in it or fewer
		*/
		bool IsColliding(void);
		/*
		USAGE: Subdivides the octant
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Subdivide(void);
		/*
		USAGE: Undivides the octant
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Undivide(void);
		/*
		USAGE: Parameterized Constructor
		ARGUMENTS: Center for the new octant, and size of the new octant
		OUTPUT: ---
		*/
		MyOctant(vector3 a_v3Center, float a_fSize);
		/*
		Usage: Constructor
		Arguments: ---
		Output: class object instance
		*/
		MyOctant(void);
		/*
		Usage: Copy Constructor
		Arguments: class object to copy
		Output: class object instance
		*/
		MyOctant(MyOctant const& other);
		/*
		Usage: Copy Assignment Operator
		Arguments: class object to copy
		Output: ---
		*/
		MyOctant& operator=(MyOctant const& other);
		/*
		Usage: Destructor
		Arguments: ---
		Output: ---
		*/
		~MyOctant(void);

		/*
		Usage: Changes object contents for other object's
		Arguments: other -> object to swap content from
		Output: ---
		*/
		void Swap(MyOctant& other);

		/*
		Usage: Gets data member
		Arguments: ---
		Output: data
		*/
		int GetData(void);
		/*
		Usage: Sets data member
		Arguments: int a_nData = 1 -> data to set
		Output: ---
		*/
		void SetData(int a_nData = 1);
		/* Property */
		//__declspec(property(get = GetData, put = SetData)) int Data;

		/*
		Usage: adds data on vector
		Arguments: int a_nData -> data to add
		Output: ---
		*/
		void SetDataOnVector(int a_nData);
		/*
		Usage: gets data on vector at the specified entry
		Arguments: int a_nIndex -> entry index
		Output: entry's content
		*/
		int& GetDataOnVector(int a_nIndex);

	private:
		/*
		Usage: Deallocates member fields
		Arguments: ---
		Output: ---
		*/
		void Release(void);
		/*
		Usage: Allocates member fields
		Arguments: ---
		Output: ---
		*/
		void Init(void);
	};//class

} //namespace Simplex

#endif //__MYOCTANT_H_

/*
USAGE:
ARGUMENTS: ---
OUTPUT: ---
*/