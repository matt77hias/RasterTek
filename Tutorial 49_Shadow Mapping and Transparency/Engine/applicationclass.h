////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;
const float SHADOWMAP_DEPTH = 50.0f;
const float SHADOWMAP_NEAR = 1.0f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "d3dclass.h"
#include "timerclass.h"
#include "positionclass.h"
#include "cameraclass.h"
#include "lightclass.h"
#include "modelclass.h"
#include "treeclass.h"
#include "rendertextureclass.h"
#include "depthshaderclass.h"
#include "transparentdepthshaderclass.h"
#include "shadowshaderclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleMovementInput(float);
	void UpdateLighting();
	bool Render();
	bool RenderSceneToTexture();

private:
	InputClass* m_Input;
	D3DClass* m_Direct3D;
	TimerClass* m_Timer;
	PositionClass* m_Position;
	CameraClass* m_Camera;
	LightClass* m_Light;
	ModelClass* m_GroundModel;
	TreeClass* m_Tree;
	RenderTextureClass* m_RenderTexture;
	DepthShaderClass* m_DepthShader;
	TransparentDepthShaderClass* m_TransparentDepthShader;
	ShadowShaderClass* m_ShadowShader;
};

#endif