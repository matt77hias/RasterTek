////////////////////////////////////////////////////////////////////////////////
// Filename: viewpointclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _VIEWPOINTCLASS_H_
#define _VIEWPOINTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: ViewPointClass
////////////////////////////////////////////////////////////////////////////////
class ViewPointClass
{
public:
	ViewPointClass();
	ViewPointClass(const ViewPointClass&);
	~ViewPointClass();

	void SetPosition(float, float, float);
	void SetLookAt(float, float, float);
	void SetProjectionParameters(float, float, float, float);

	void GenerateViewMatrix();
	void GenerateProjectionMatrix();

	void GetViewMatrix(D3DXMATRIX&);
	void GetProjectionMatrix(D3DXMATRIX&);

private:
	D3DXVECTOR3 m_position, m_lookAt;
	D3DXMATRIX m_viewMatrix, m_projectionMatrix;
	float m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane;
};

#endif