#include "AppClass.h"
void Application::InitVariables(void)
{

	//init the array of meshes
	m_pMeshes = new MyMesh[numCubes];
	//have each mesh generate a cube
	for (size_t i = 0; i < numCubes; i++)
	{
		m_pMeshes[i].GenerateCube(1.0f, C_WHITE);
	}

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
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	static float value = 0.0f;

	//move all the cubes to where they need to be
	matrix4 m4Translate;
	for (size_t i = 0; i < numCubes; i++)
	{
		switch (i)
		{
		case 0:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-2.0f + value, -3.0f, 0.0f));
			break;
		case 1:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-1.0f + value, -3.0f, 0.0f));
			break;
		case 2:
			m4Translate = glm::translate(IDENTITY_M4, vector3(1.0f + value, -3.0f, 0.0f));
			break;
		case 3:
			m4Translate = glm::translate(IDENTITY_M4, vector3(2.0f + value, -3.0f, 0.0f));
			break;
		case 4:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-5.0f + value, -2.0f, 0.0f));
			break;
		case 5:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-3.0f + value, -2.0f, 0.0f));
			break;
		case 6:
			m4Translate = glm::translate(IDENTITY_M4, vector3(3.0f + value, -2.0f, 0.0f));
			break;
		case 7:
			m4Translate = glm::translate(IDENTITY_M4, vector3(5.0f + value, -2.0f, 0.0f));
			break;
		case 8:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-5.0f + value, -1.0f, 0.0f));
			break;
		case 9:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-3.0f + value, -1.0f, 0.0f));
			break;
		case 10:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-2.0f + value, -1.0f, 0.0f));
			break;
		case 11:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-1.0f + value, -1.0f, 0.0f));
			break;
		case 12:
			m4Translate = glm::translate(IDENTITY_M4, vector3(0.0f + value, -1.0f, 0.0f));
			break;
		case 13:
			m4Translate = glm::translate(IDENTITY_M4, vector3(1.0f + value, -1.0f, 0.0f));
			break;
		case 14:
			m4Translate = glm::translate(IDENTITY_M4, vector3(2.0f + value, -1.0f, 0.0f));
			break;
		case 15:
			m4Translate = glm::translate(IDENTITY_M4, vector3(3.0f + value, -1.0f, 0.0f));
			break;
		case 16:
			m4Translate = glm::translate(IDENTITY_M4, vector3(5.0f + value, -1.0f, 0.0f));
			break;
		case 17:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-5.0f + value, 0.0f, 0.0f));
			break;
		case 18:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-4.0f + value, 0.0f, 0.0f));
			break;
		case 19:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-3.0f + value, 0.0f, 0.0f));
			break;
		case 20:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-2.0f + value, 0.0f, 0.0f));
			break;
		case 21:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-1.0f + value, 0.0f, 0.0f));
			break;
		case 22:
			m4Translate = glm::translate(IDENTITY_M4, vector3(0.0f + value, 0.0f, 0.0f));
			break;
		case 23:
			m4Translate = glm::translate(IDENTITY_M4, vector3(1.0f + value, 0.0f, 0.0f));
			break;
		case 24:
			m4Translate = glm::translate(IDENTITY_M4, vector3(2.0f + value, 0.0f, 0.0f));
			break;
		case 25:
			m4Translate = glm::translate(IDENTITY_M4, vector3(3.0f + value, 0.0f, 0.0f));
			break;
		case 26:
			m4Translate = glm::translate(IDENTITY_M4, vector3(4.0f + value, 0.0f, 0.0f));
			break;
		case 27:
			m4Translate = glm::translate(IDENTITY_M4, vector3(5.0f + value, 0.0f, 0.0f));
			break;
		case 28:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-4.0f + value, 1.0f, 0.0f));
			break;
		case 29:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-3.0f + value, 1.0f, 0.0f));
			break;
		case 30:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-1.0f + value, 1.0f, 0.0f));
			break;
		case 31:
			m4Translate = glm::translate(IDENTITY_M4, vector3(0.0f + value, 1.0f, 0.0f));
			break;
		case 32:
			m4Translate = glm::translate(IDENTITY_M4, vector3(1.0f + value, 1.0f, 0.0f));
			break;
		case 33:
			m4Translate = glm::translate(IDENTITY_M4, vector3(3.0f + value, 1.0f, 0.0f));
			break;
		case 34:
			m4Translate = glm::translate(IDENTITY_M4, vector3(4.0f + value, 1.0f, 0.0f));
			break;
		case 35:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-3.0f + value, 2.0f, 0.0f));
			break;
		case 36:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-2.0f + value, 2.0f, 0.0f));
			break;
		case 37:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-1.0f + value, 2.0f, 0.0f));
			break;
		case 38:
			m4Translate = glm::translate(IDENTITY_M4, vector3(0.0f + value, 2.0f, 0.0f));
			break;
		case 39:
			m4Translate = glm::translate(IDENTITY_M4, vector3(1.0f + value, 2.0f, 0.0f));
			break;
		case 40:
			m4Translate = glm::translate(IDENTITY_M4, vector3(2.0f + value, 2.0f, 0.0f));
			break;
		case 41:
			m4Translate = glm::translate(IDENTITY_M4, vector3(3.0f + value, 2.0f, 0.0f));
			break;
		case 42:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-2.0f + value, 3.0f, 0.0f));
			break;
		case 43:
			m4Translate = glm::translate(IDENTITY_M4, vector3(2.0f + value, 3.0f, 0.0f));
			break;
		case 44:
			m4Translate = glm::translate(IDENTITY_M4, vector3(-3.0f + value, 4.0f, 0.0f));
			break;
		case 45:
			m4Translate = glm::translate(IDENTITY_M4, vector3(3.0f + value, 4.0f, 0.0f));
			break;
		default:
			break;
		}
		m_pMeshes[i].Render(m4Projection, m4View, m4Translate);
	}
	value += 0.01f;
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
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
	//deallocate the meshes
	SafeDelete(m_pMeshes);

	//release GUI
	ShutdownGUI();
}