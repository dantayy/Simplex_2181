#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);
	
	//allocate the primitive
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateTorus(3.0f, 2.0f, 6, 6, C_RED);

	//allocate the primitive
	m_pMesh2 = new MyMesh();
	m_pMesh2->GenerateTorus(3.0f, 2.0f, 8, 8, C_GREEN);

	m_pMesh3 = new MyMesh();
	m_pMesh3->GenerateCone(0.5f, 1.0f, 6, C_BLUE);

	//create a new camera
	m_pCamera = new MyCamera();

	//projection used
	m_uProjection = 1;
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	//Clear the screen
	ClearScreen();
	
	//draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	//vars for projection matrix
	float fovy = 45.0f; //field of view
	float aspect = m_pSystem->GetWindowWidth() / m_pSystem->GetWindowHeight(); //aspect ratio
	float zNear = 0.0001f; //near clippling plane
	float zFar = 1000.0f; //far clipping plane
	//vars for view matrix
	vector3 v3Position = vector3(0.0f, 0.0f, 10.0f);
	vector3 v3Target = vector3(0.0f, 0.0f, -10.0f);
	vector3 v3Up = vector3(0.0f, 1.0f, 0.0f);

	//calculate view and projection
	switch (m_uProjection)
	{
	default:
	case 1:
		//m_pCamera->ResetCamera();
		break;
	case 2:
		fovy = 90.0f;
		break;
	case 3:
		v3Position = vector3(5.0f, 0.0f, 0.0f); //move camera to the right
		v3Target = vector3(-5.0f, 0.0f, 0.0f); //have it facing the left
		v3Up = vector3(0.0f, 0.0f, -1.0f); //have it's "up" point into the z axis
		break;
	case 4:
		v3Position = vector3(0.0f, 0.0f, -10.0f); //move the camera behind the three objects
		v3Target = vector3(0.0f, 0.0f, 10.0f); //turn the camera to face the three objects again, but from behind
		break;
	case 5:
		v3Position = vector3(0.0f, 0.0f, -10.0f); //move the camera behind the three objects
		v3Target = vector3(0.0f, 0.0f, 10.0f); //turn the camera to face the three objects again, but from behind
		zNear = 0.08f; //move the near plane up so the closest object doesn't render (???)
		break;
	case 6:
		v3Position = vector3(0.0f, 0.0f, -10.0f); //move the camera behind the three objects
		v3Target = vector3(0.0f, 0.0f, 10.0f); //turn the camera to face the three objects again, but from behind
		zNear = 0.06f; //move the far plane towards you so the object farthest from you doesn't render (???)
		break;
	case 7:
		v3Up = vector3(0.0f, -1.0f, 0.0f); //turn the camera upside-down
		break;
	}

	m4Projection = glm::perspective(fovy, aspect, zNear, zFar);
	m4View = glm::lookAt(v3Position, v3Target, v3Up);

	m_pCameraMngr->SetProjectionMatrix(m4Projection);
	m_pCameraMngr->SetViewMatrix(m4View);

	m_pCamera->CalculateProjectionMatrix();
	m_pCamera->CalculateViewMatrix();

	//draw the primitive
	m_pMesh1->Render(m_pCamera, glm::rotate(IDENTITY_M4, 1.5708f, AXIS_X));
	m_pMesh2->Render(m_pCamera, glm::translate(IDENTITY_M4, vector3(0.0f,0.0f,-5.0f)) * glm::rotate(IDENTITY_M4, 1.5708f, AXIS_X));
	m_pMesh3->Render(m_pCamera, glm::translate(vector3(0.0f, 0.0f, -10.0f)));

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release primitive
	SafeDelete(m_pMesh1);
	SafeDelete(m_pMesh2);
	SafeDelete(m_pMesh3);
	
	//release the camera
	SafeDelete(m_pCamera);
	
	//release GUI
	ShutdownGUI();
}
