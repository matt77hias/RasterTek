////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetDirection(float, float, float);
	void SetPosition(float, float, float);
	void SetLookAt(float, float, float);

	D3DXVECTOR3 GetDirection();
	D3DXVECTOR3 GetPosition();

	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float, float);
	void GenerateOrthoMatrix(float, float, float, float);

	void GetViewMatrix(D3DXMATRIX&);
	void GetProjectionMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

private:
	D3DXVECTOR3 m_direction, m_position, m_lookAt;
	D3DXMATRIX m_viewMatrix, m_projectionMatrix, m_orthoMatrix;
};

#endif