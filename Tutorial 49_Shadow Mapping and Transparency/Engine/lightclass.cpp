////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightclass.h"


LightClass::LightClass()
{
}


LightClass::LightClass(const LightClass& other)
{
}


LightClass::~LightClass()
{
}


void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
	return;
}


void LightClass::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);
	return;
}


void LightClass::SetLookAt(float x, float y, float z)
{
	m_lookAt.x = x;
	m_lookAt.y = y;
	m_lookAt.z = z;
	return;
}


D3DXVECTOR3 LightClass::GetDirection()
{
	return m_direction;
}


D3DXVECTOR3 LightClass::GetPosition()
{
	return m_position;
}


void LightClass::GenerateViewMatrix()
{
	D3DXVECTOR3 up;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Create the view matrix from the three vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_lookAt, &up);
	
	return;
}


void LightClass::GenerateProjectionMatrix(float screenDepth, float screenNear)
{
	float fieldOfView, screenAspect;


	// Setup field of view and screen aspect for a square light source.
	fieldOfView = (float)D3DX_PI / 2.0f;
	screenAspect = 1.0f;

	// Create the projection matrix for the light.
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	return;
}


void LightClass::GenerateOrthoMatrix(float width, float height, float screenDepth, float screenNear)
{
	// Create the projection matrix for the light.
	D3DXMatrixOrthoLH(&m_orthoMatrix, width, height, screenNear, screenDepth);

	return;
}


void LightClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}


void LightClass::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}


void LightClass::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}