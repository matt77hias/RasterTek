////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightclass.h"
#include "rendertextureclass.h"
#include "lightshaderclass.h"
#include "refractionshaderclass.h"
#include "watershaderclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	bool Render();

private:
	bool RenderRefractionToTexture();
	bool RenderReflectionToTexture();
	bool RenderScene();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass *m_GroundModel, *m_WallModel, *m_BathModel, *m_WaterModel;
	LightClass* m_Light;
	RenderTextureClass *m_RefractionTexture, *m_ReflectionTexture;
	LightShaderClass* m_LightShader;
	RefractionShaderClass* m_RefractionShader;
	WaterShaderClass* m_WaterShader;
	float m_waterHeight, m_waterTranslation;
};

#endif