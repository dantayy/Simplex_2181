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

	//set up the root node
	root = new MyOctant();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//update collisions
	m_pEntityMngr->Update();

	//Is the first person camera active?
	CameraRotation();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//display octree
	root->Display();

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

	//delete the octants
	while (m_uOctantLevels > 1)
	{
		root->Undivide();
		m_uOctantLevels--;
	}
	SafeDelete(root);
}