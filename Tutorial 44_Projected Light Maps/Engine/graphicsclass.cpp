////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_GroundModel = 0;
	m_CubeModel = 0;
	m_Light = 0;
	m_ProjectionShader = 0;
	m_ProjectionTexture = 0;
	m_ViewPoint = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position and rotation of the camera.
	m_Camera->SetPosition(0.0f, 7.0f, -10.0f);
	m_Camera->SetRotation(35.0f, 0.0f, 0.0f);
	
	// Create the ground model object.
	m_GroundModel = new ModelClass;
	if(!m_GroundModel)
	{
		return false;
	}

	// Initialize the ground model object.
	result = m_GroundModel->Initialize(m_D3D->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/stone.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the ground model object.", L"Error", MB_OK);
		return false;
	}


	// Create the cube model object.
	m_CubeModel = new ModelClass;
	if(!m_CubeModel)
	{
		return false;
	}

	// Initialize the cube model object.
	result = m_CubeModel->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/seafloor.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the cube model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetPosition(2.0f, 5.0f, -2.0f);

	// Create the projection shader object.
	m_ProjectionShader = new ProjectionShaderClass;
	if(!m_ProjectionShader)
	{
		return false;
	}

	// Initialize the projection shader object.
	result = m_ProjectionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the projection shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the projection texture object.
	m_ProjectionTexture = new TextureClass;
	if(!m_ProjectionTexture)
	{
		return false;
	}

	// Initialize the projection texture object.
	result = m_ProjectionTexture->Initialize(m_D3D->GetDevice(), L"../Engine/data/grate.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the projection texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the view point object.
	m_ViewPoint = new ViewPointClass;
	if(!m_ViewPoint)
	{
		return false;
	}

	// Initialize the view point object.
	m_ViewPoint->SetPosition(2.0f, 5.0f, -2.0f);
	m_ViewPoint->SetLookAt(0.0f, 0.0f, 0.0f);
	m_ViewPoint->SetProjectionParameters((float)(D3DX_PI / 2.0f), 1.0f, 0.1f, 100.0f);
	m_ViewPoint->GenerateViewMatrix();
	m_ViewPoint->GenerateProjectionMatrix();

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the view point object.
	if(m_ViewPoint)
	{
		delete m_ViewPoint;
		m_ViewPoint = 0;
	}

	// Release the projection texture object.
	if(m_ProjectionTexture)
	{
		m_ProjectionTexture->Shutdown();
		delete m_ProjectionTexture;
		m_ProjectionTexture = 0;
	}

	// Release the projection shader object.
	if(m_ProjectionShader)
	{
		m_ProjectionShader->Shutdown();
		delete m_ProjectionShader;
		m_ProjectionShader = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the cube model object.
	if(m_CubeModel)
	{
		m_CubeModel->Shutdown();
		delete m_CubeModel;
		m_CubeModel = 0;
	}

	// Release the ground model object.
	if(m_GroundModel)
	{
		m_GroundModel->Shutdown();
		delete m_GroundModel;
		m_GroundModel = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;


	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	D3DXMATRIX viewMatrix2, projectionMatrix2;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Get the view and projection matrices from the view point object.
	m_ViewPoint->GetViewMatrix(viewMatrix2);
	m_ViewPoint->GetProjectionMatrix(projectionMatrix2);

	// Setup the translation for the ground model.
	D3DXMatrixTranslation(&worldMatrix, 0.0f, 1.0f, 0.0f);
	
	// Render the ground model using the projection shader.
	m_GroundModel->Render(m_D3D->GetDeviceContext());
	result = m_ProjectionShader->Render(m_D3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
										m_GroundModel->GetTexture(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetPosition(), 
										viewMatrix2, projectionMatrix2, m_ProjectionTexture->GetTexture());
	if(!result)
	{
		return false;
	}

	// Reset the world matrix and setup the translation for the cube model.
	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, 0.0f, 2.0f, 0.0f);

	// Render the cube model using the projection shader.
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	result = m_ProjectionShader->Render(m_D3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
										m_CubeModel->GetTexture(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetPosition(), 
										viewMatrix2, projectionMatrix2, m_ProjectionTexture->GetTexture());
	if(!result)
	{
		return false;
	}
							   
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}