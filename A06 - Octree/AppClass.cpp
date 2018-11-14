#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 0.0f, 100.0f), //Position
		vector3(0.0f, 0.0f, 99.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

#ifdef DEBUG
	uint uInstances = 900;
#else
	uint uInstances = 1849;
#endif
	int nSquare = static_cast<int>(std::sqrt(uInstances));
	m_uObjects = nSquare * nSquare;
	uint uIndex = -1;
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			uIndex++;
			m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
			vector3 v3Position = vector3(glm::sphericalRand(34.0f));
			matrix4 m4Position = glm::translate(v3Position);
			m_pEntityMngr->SetModelMatrix(m4Position);
		}
	}
	m_uOctantLevels = 1;
	/*
	- make a tree where if a node (box) has children (sub-boxes), it has 8 children (1 for each +/-x, +/-y, & +/-z octant space)
	- perhaps have each node store its min and max global values for reference when inserting/checking for objects
	- only leaves in the tree should have objects assigned to them (perhaps generate the tree first then add the objects?)
	- check for lack of children to determine leafhood by *traversing* the tree and whenever a node with no children is hit, add objects to its object array before stepping out
	- to add an object to leaves, check each leaf cube space against the 8 corners of the object and if any of the corners are inside, add them to those leaves
	- do this for every object in the scene
	- to check to see if two objects are in the same leaf/octant, simply check all the leaves for both of them (i.e. do a for loop *in each leaf* looking for first object, then if you find that one then for loop again looking for the second one)
	- tree insertion: check to see if any leaf has more than a specified # of objects in it, if it does, create the 8 children (regenerating the tree in the process(how?))
	*/
	//find the upper and lower bounds of the space taken up by objects in this scene
	vector3 rootMin = vector3();
	vector3 rootMax = vector3();
	//get the initial values for absolute mins and maxes of the objects
	for (size_t i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		if (rootMax.x < m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMaxGlobal().x) rootMax.x = m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMaxGlobal().x;
		else if (rootMin.x > m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMinGlobal().x) rootMin.x = m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMinGlobal().x;

		if (rootMax.y < m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMaxGlobal().y) rootMax.y = m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMaxGlobal().y;
		else if (rootMin.y > m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMinGlobal().y) rootMin.y = m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMinGlobal().y;

		if (rootMax.z < m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMaxGlobal().z) rootMax.z = m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMaxGlobal().z;
		else if (rootMin.z > m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMinGlobal().z) rootMin.z = m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMinGlobal().z;
	}

	//make the root octant square by equalizing the mins and maxes
	if (std::abs(rootMin.x) > rootMax.x) rootMax.x = std::abs(rootMin.x);
	else rootMin.x = -rootMax.x;
	if (std::abs(rootMin.y) > rootMax.y) rootMax.y = std::abs(rootMin.y);
	else rootMin.y = -rootMax.y;
	if (std::abs(rootMin.z) > rootMax.z) rootMax.z = std::abs(rootMin.z);
	else rootMin.z = -rootMax.z;

	//set the root octant node based on the min and max calculated above
	root = MyOctant(rootMin, rootMax);

	//set the oct tree with the root and entity manager that have been initialized
	mainTree = MyOctTree(&root, m_pEntityMngr);
	mainTree.AddObjs(&root);
	//check collisions
	for (size_t i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		mainTree.CollisionInLeaf(&root, m_pEntityMngr->GetEntity(i));
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	//Update Entity Manager
	//check collisions
	for (size_t i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		mainTree.CollisionInLeaf(&root, m_pEntityMngr->GetEntity(i));
	}

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//display octree
	//m_pRoot->Display();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui,
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}