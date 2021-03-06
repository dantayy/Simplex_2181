#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();

}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//create an array of vec3's for base point storage
	conePoints = new vector3[a_nSubdivisions];
	//calculate the positions for the base
	for (size_t i = 0; i < a_nSubdivisions; i++)
	{
		conePoints[i] = { a_fRadius * cos(((2 * PI) / a_nSubdivisions) * i), -a_fHeight/2, a_fRadius * sin(((2 * PI) / a_nSubdivisions) * i) };
	}
	//add all the tris for the base (except for the last one)
	for (size_t i = 1; i < a_nSubdivisions; i++)
	{
		AddTri({ 0,-a_fHeight / 2,0 }, conePoints[i - 1], conePoints[i]);
	}
	//add the last tri for the base
	AddTri({ 0,-a_fHeight/2,0 }, conePoints[a_nSubdivisions - 1], conePoints[0]);
	//add tris from the base circle to the top of the cone (except for the last one)
	for (size_t i = 1; i < a_nSubdivisions; i++)
	{
		//make sure all faces are drawn outwards, flipping the vertex order when needed
		if (i / (2 * PI) > PI)
			AddTri({ 0,a_fHeight/2,0 }, conePoints[i - 1], conePoints[i]);
		else
			AddTri({ 0,a_fHeight/2,0 }, conePoints[i], conePoints[i - 1]);
	}
	//add the last tri for the side of the cone
	AddTri({ 0,a_fHeight/2,0 }, conePoints[0], conePoints[a_nSubdivisions - 1]);

	//deallocate memory on the heap
	delete[] conePoints;

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//create an array of vec3's for base point storage
	cylinderBasePoints = new vector3[a_nSubdivisions];
	//create an array of vec3's for top point storage
	cylinderTopPoints = new vector3[a_nSubdivisions];
	//calculate the positions for the base and top
	for (size_t i = 0; i < a_nSubdivisions; i++)
	{
		cylinderBasePoints[i] = { a_fRadius * cos(((2 * PI) / a_nSubdivisions) * i), -a_fHeight / 2, a_fRadius * sin(((2 * PI) / a_nSubdivisions) * i) };
		cylinderTopPoints[i] = { a_fRadius * cos(((2 * PI) / a_nSubdivisions) * i), a_fHeight / 2, a_fRadius * sin(((2 * PI) / a_nSubdivisions) * i) };
	}
	//add all the tris and quads for the base, top, and what connects them (except for the last ones)
	for (size_t i = 1; i < a_nSubdivisions; i++)
	{
		AddTri({ 0,-a_fHeight / 2,0 }, cylinderBasePoints[i - 1], cylinderBasePoints[i]);
		AddTri({ 0,a_fHeight / 2,0 }, cylinderTopPoints[i], cylinderTopPoints[i - 1]);
		//make sure all faces are drawn outwards, flipping the vertex order when needed
		if(i/(2*PI) > PI)
			AddQuad(cylinderBasePoints[i - 1], cylinderBasePoints[i], cylinderTopPoints[i - 1], cylinderTopPoints[i]);
		else
			AddQuad(cylinderBasePoints[i], cylinderBasePoints[i - 1], cylinderTopPoints[i], cylinderTopPoints[i - 1]);
	}
	//add the last tri for the base
	AddTri({ 0,-a_fHeight / 2,0 }, cylinderBasePoints[a_nSubdivisions - 1], cylinderBasePoints[0]);
	//add the last tri for the top
	AddTri({ 0,a_fHeight / 2,0 }, cylinderTopPoints[0], cylinderTopPoints[a_nSubdivisions - 1]);
	//add the last quad
	AddQuad(cylinderBasePoints[0], cylinderBasePoints[a_nSubdivisions - 1], cylinderTopPoints[0], cylinderTopPoints[a_nSubdivisions - 1]);

	//deallocate memory on the heap
	delete[] cylinderBasePoints;
	delete[] cylinderTopPoints;

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//create an array of vec3's for inner base point storage
	innerTubeBasePoints = new vector3[a_nSubdivisions];
	//create an array of vec3's for inner top point storage
	innerTubeTopPoints = new vector3[a_nSubdivisions];
	//create an array of vec3's for outer base point storage
	outerTubeBasePoints = new vector3[a_nSubdivisions];
	//create an array of vec3's for outer top point storage
	outerTubeTopPoints = new vector3[a_nSubdivisions];

	//calculate the positions for the inner and outer bases and tops
	for (size_t i = 0; i < a_nSubdivisions; i++)
	{
		innerTubeBasePoints[i] = { a_fInnerRadius * cos(((2 * PI) / a_nSubdivisions) * i), -a_fHeight / 2, a_fInnerRadius * sin(((2 * PI) / a_nSubdivisions) * i) };
		innerTubeTopPoints[i] = { a_fInnerRadius * cos(((2 * PI) / a_nSubdivisions) * i), a_fHeight / 2, a_fInnerRadius * sin(((2 * PI) / a_nSubdivisions) * i) };
		outerTubeBasePoints[i] = { a_fOuterRadius * cos(((2 * PI) / a_nSubdivisions) * i), -a_fHeight / 2, a_fOuterRadius * sin(((2 * PI) / a_nSubdivisions) * i) };
		outerTubeTopPoints[i] = { a_fOuterRadius * cos(((2 * PI) / a_nSubdivisions) * i), a_fHeight / 2, a_fOuterRadius * sin(((2 * PI) / a_nSubdivisions) * i) };
	}
	//add all the quads for the tube (except for the last ones)
	for (size_t i = 1; i < a_nSubdivisions; i++)
	{
		AddQuad(outerTubeBasePoints[i - 1], outerTubeBasePoints[i], innerTubeBasePoints[i - 1], innerTubeBasePoints[i]);
		AddQuad(innerTubeTopPoints[i - 1], innerTubeTopPoints[i], outerTubeTopPoints[i - 1], outerTubeTopPoints[i]);
		AddQuad(outerTubeTopPoints[i - 1], outerTubeTopPoints[i], outerTubeBasePoints[i - 1], outerTubeBasePoints[i]);
		AddQuad(innerTubeBasePoints[i - 1], innerTubeBasePoints[i], innerTubeTopPoints[i - 1], innerTubeTopPoints[i]);
	}
	//add the last quads
	AddQuad(outerTubeBasePoints[a_nSubdivisions - 1], outerTubeBasePoints[0], innerTubeBasePoints[a_nSubdivisions - 1], innerTubeBasePoints[0]);
	AddQuad(innerTubeTopPoints[a_nSubdivisions - 1], innerTubeTopPoints[0], outerTubeTopPoints[a_nSubdivisions - 1], outerTubeTopPoints[0]);
	AddQuad(outerTubeTopPoints[a_nSubdivisions - 1], outerTubeTopPoints[0], outerTubeBasePoints[a_nSubdivisions - 1], outerTubeBasePoints[0]);
	AddQuad(innerTubeBasePoints[a_nSubdivisions - 1], innerTubeBasePoints[0], innerTubeTopPoints[a_nSubdivisions - 1], innerTubeTopPoints[0]);
	
	//deallocate memory on the heap
	delete[] outerTubeBasePoints;
	delete[] innerTubeBasePoints;
	delete[] outerTubeTopPoints;
	delete[] innerTubeTopPoints;

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 3)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//create an array of vec3's for lower sphere ring point storage
	lowerSpherePoints = new vector3[a_nSubdivisions];
	//create an array of vec3's for middle sphere ring point storage
	middleSpherePoints = new vector3[a_nSubdivisions];
	//create an array of vec3's for upper sphere ring point storage
	upperSpherePoints = new vector3[a_nSubdivisions];

	//calculate positions for the three main rings of the sphere (using # of subdivisions to determine how well defined each ring is)
	for (size_t i = 0; i < a_nSubdivisions; i++)
	{
		lowerSpherePoints[i] = { a_fRadius * (sqrt(2)/2) * cos(((2 * PI) / a_nSubdivisions) * i), -a_fRadius / 2, a_fRadius / 2 * sin(((2 * PI) / a_nSubdivisions) * i) };
		middleSpherePoints[i] = { a_fRadius * cos(((2 * PI) / a_nSubdivisions) * i), 0, a_fRadius * sin(((2 * PI) / a_nSubdivisions) * i) };
		upperSpherePoints[i] = { a_fRadius * (sqrt(2) / 2) * cos(((2 * PI) / a_nSubdivisions) * i), a_fRadius / 2, a_fRadius / 2 * sin(((2 * PI) / a_nSubdivisions) * i) };
	}
	//add all the tris and quads for the sphere
	for (size_t i = 1; i < a_nSubdivisions; i++)
	{
		//connecting bottom point of sphere to lower ring
		AddTri({ 0,-a_fRadius,0 }, lowerSpherePoints[i - 1], lowerSpherePoints[i]);
		//connecting top point of sphere to upper ring
		AddTri({ 0,a_fRadius,0 }, upperSpherePoints[i], upperSpherePoints[i - 1]);
		//make sure all faces are drawn outwards, flipping the vertex order when needed
		if (i / (2 * PI) > PI)
		{
			//connecting lower ring to middle ring
			AddQuad(lowerSpherePoints[i - 1], lowerSpherePoints[i], middleSpherePoints[i - 1], middleSpherePoints[i]);
			//connecting middle ring to upper ring
			AddQuad(middleSpherePoints[i - 1], middleSpherePoints[i], upperSpherePoints[i - 1], upperSpherePoints[i]);
		}
		else
		{
			//connecting lower ring to middle ring
			AddQuad(lowerSpherePoints[i], lowerSpherePoints[i - 1], middleSpherePoints[i], middleSpherePoints[i - 1]);
			//connecting middle ring to upper ring
			AddQuad(middleSpherePoints[i], middleSpherePoints[i - 1], upperSpherePoints[i], upperSpherePoints[i - 1]);
		}
	}
	//add the last tri for the bottom
	AddTri({ 0,-a_fRadius,0 }, lowerSpherePoints[a_nSubdivisions - 1], lowerSpherePoints[0]);
	//add the last tri for the top
	AddTri({ 0,a_fRadius,0 }, upperSpherePoints[0], upperSpherePoints[a_nSubdivisions - 1]);
	//add the last quads
	AddQuad(lowerSpherePoints[0], lowerSpherePoints[a_nSubdivisions - 1], middleSpherePoints[0], middleSpherePoints[a_nSubdivisions - 1]);
	AddQuad(middleSpherePoints[0], middleSpherePoints[a_nSubdivisions - 1], upperSpherePoints[0], upperSpherePoints[a_nSubdivisions - 1]);

	//deallocate memory on the heap
	delete[] lowerSpherePoints;
	delete[] middleSpherePoints;
	delete[] upperSpherePoints;

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}