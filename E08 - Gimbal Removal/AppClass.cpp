#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	m_pMesh->GenerateCone(2.0f, 5.0f, 3, C_WHITE);
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

	////make a quaternion for the rotation by combining all of the individual axis rotations together
	//quaternion xRot = quaternion(vector3(glm::radians(m_v3Rotation.x), 0, 0));
	//quaternion yRot = quaternion(vector3(0, glm::radians(m_v3Rotation.y), 0));
	//quaternion zRot = quaternion(vector3(0, 0, glm::radians(m_v3Rotation.z)));
	//quaternion totalRot = xRot * yRot * zRot;

	////SLERP between where the object currently is and where it should be
	//m_m4Model = glm::mix(m_m4Model, ToMatrix4(totalRot), 1.0f);

	//m_pMesh->Render(m4Projection, m4View, ToMatrix4(m_m4Model));

	m_qOrientation = m_qOrientation * glm::angleAxis(glm::radians(1.0f), vector3(glm::radians(m_v3Rotation.x), glm::radians(m_v3Rotation.y), glm::radians(m_v3Rotation.z)));
	m_pMesh->Render(m4Projection, m4View, ToMatrix4(m_qOrientation));
	
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
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}