////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_CubeModel = 0;
	m_GroundModel = 0;
	m_SphereModel = 0;
	m_Light = 0;
	m_RenderTexture = 0;
	m_DepthShader = 0;	
	m_BlackWhiteRenderTexture = 0;
	m_ShadowShader = 0;
	m_DownSampleTexure = 0;
	m_SmallWindow = 0;
	m_TextureShader = 0;
	m_HorizontalBlurTexture = 0;
	m_HorizontalBlurShader = 0;
	m_VerticalBlurTexture = 0;
	m_VerticalBlurShader = 0;
	m_UpSampleTexure = 0;
	m_FullScreenWindow = 0;
	m_SoftShadowShader = 0;
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
	int downSampleWidth, downSampleHeight;


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

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->RenderBaseViewMatrix();

	// Create the cube model object.
	m_CubeModel = new ModelClass;
	if(!m_CubeModel)
	{
		return false;
	}

	// Initialize the cube model object.
	result = m_CubeModel->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/wall01.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the cube model object.", L"Error", MB_OK);
		return false;
	}

	// Set the position for the cube model.
	m_CubeModel->SetPosition(-2.0f, 2.0f, 0.0f);

	// Create the sphere model object.
	m_SphereModel = new ModelClass;
	if(!m_SphereModel)
	{
		return false;
	}

	// Initialize the sphere model object.
	result = m_SphereModel->Initialize(m_D3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/ice.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the sphere model object.", L"Error", MB_OK);
		return false;
	}

	// Set the position for the sphere model.
	m_SphereModel->SetPosition(2.0f, 2.0f, 0.0f);

	// Create the ground model object.
	m_GroundModel = new ModelClass;
	if(!m_GroundModel)
	{
		return false;
	}

	// Initialize the ground model object.
	result = m_GroundModel->Initialize(m_D3D->GetDevice(), "../Engine/data/plane01.txt", L"../Engine/data/metal001.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the ground model object.", L"Error", MB_OK);
		return false;
	}

	// Set the position for the ground model.
	m_GroundModel->SetPosition(0.0f, 1.0f, 0.0f);

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetLookAt(0.0f, 0.0f, 0.0f);
	m_Light->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if(!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the depth shader object.
	m_DepthShader = new DepthShaderClass;
	if(!m_DepthShader)
	{
		return false;
	}

	// Initialize the depth shader object.
	result = m_DepthShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the depth shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the black and white render to texture object.
	m_BlackWhiteRenderTexture = new RenderTextureClass;
	if(!m_BlackWhiteRenderTexture)
	{
		return false;
	}

	// Initialize the black and white render to texture object.
	result = m_BlackWhiteRenderTexture->Initialize(m_D3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the black and white render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the shadow shader object.
	m_ShadowShader = new ShadowShaderClass;
	if(!m_ShadowShader)
	{
		return false;
	}

	// Initialize the shadow shader object.
	result = m_ShadowShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the shadow shader object.", L"Error", MB_OK);
		return false;
	}

	// Set the size to sample down to.
	downSampleWidth = SHADOWMAP_WIDTH / 2;
	downSampleHeight = SHADOWMAP_HEIGHT / 2;

	// Create the down sample render to texture object.
	m_DownSampleTexure = new RenderTextureClass;
	if(!m_DownSampleTexure)
	{
		return false;
	}

	// Initialize the down sample render to texture object.
	result = m_DownSampleTexure->Initialize(m_D3D->GetDevice(), downSampleWidth, downSampleHeight, 100.0f, 1.0f);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the down sample render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the small ortho window object.
	m_SmallWindow = new OrthoWindowClass;
	if(!m_SmallWindow)
	{
		return false;
	}

	// Initialize the small ortho window object.
	result = m_SmallWindow->Initialize(m_D3D->GetDevice(), downSampleWidth, downSampleHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the small ortho window object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the horizontal blur render to texture object.
	m_HorizontalBlurTexture = new RenderTextureClass;
	if(!m_HorizontalBlurTexture)
	{
		return false;
	}

	// Initialize the horizontal blur render to texture object.
	result = m_HorizontalBlurTexture->Initialize(m_D3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, 0.1f);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the horizontal blur shader object.
	m_HorizontalBlurShader = new HorizontalBlurShaderClass;
	if(!m_HorizontalBlurShader)
	{
		return false;
	}

	// Initialize the horizontal blur shader object.
	result = m_HorizontalBlurShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the vertical blur render to texture object.
	m_VerticalBlurTexture = new RenderTextureClass;
	if(!m_VerticalBlurTexture)
	{
		return false;
	}

	// Initialize the vertical blur render to texture object.
	result = m_VerticalBlurTexture->Initialize(m_D3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, 0.1f);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the vertical blur shader object.
	m_VerticalBlurShader = new VerticalBlurShaderClass;
	if(!m_VerticalBlurShader)
	{
		return false;
	}

	// Initialize the vertical blur shader object.
	result = m_VerticalBlurShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the up sample render to texture object.
	m_UpSampleTexure = new RenderTextureClass;
	if(!m_UpSampleTexure)
	{
		return false;
	}

	// Initialize the up sample render to texture object.
	result = m_UpSampleTexure->Initialize(m_D3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, 0.1f);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the up sample render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the full screen ortho window object.
	m_FullScreenWindow = new OrthoWindowClass;
	if(!m_FullScreenWindow)
	{
		return false;
	}

	// Initialize the full screen ortho window object.
	result = m_FullScreenWindow->Initialize(m_D3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the full screen ortho window object.", L"Error", MB_OK);
		return false;
	}

	// Create the soft shadow shader object.
	m_SoftShadowShader = new SoftShadowShaderClass;
	if(!m_SoftShadowShader)
	{
		return false;
	}

	// Initialize the soft shadow shader object.
	result = m_SoftShadowShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the soft shadow shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the soft shadow shader object.
	if(m_SoftShadowShader)
	{
		m_SoftShadowShader->Shutdown();
		delete m_SoftShadowShader;
		m_SoftShadowShader = 0;
	}

	// Release the full screen ortho window object.
	if(m_FullScreenWindow)
	{
		m_FullScreenWindow->Shutdown();
		delete m_FullScreenWindow;
		m_FullScreenWindow = 0;
	}

	// Release the up sample render to texture object.
	if(m_UpSampleTexure)
	{
		m_UpSampleTexure->Shutdown();
		delete m_UpSampleTexure;
		m_UpSampleTexure = 0;
	}

	// Release the vertical blur shader object.
	if(m_VerticalBlurShader)
	{
		m_VerticalBlurShader->Shutdown();
		delete m_VerticalBlurShader;
		m_VerticalBlurShader = 0;
	}

	// Release the vertical blur render to texture object.
	if(m_VerticalBlurTexture)
	{
		m_VerticalBlurTexture->Shutdown();
		delete m_VerticalBlurTexture;
		m_VerticalBlurTexture = 0;
	}

	// Release the horizontal blur shader object.
	if(m_HorizontalBlurShader)
	{
		m_HorizontalBlurShader->Shutdown();
		delete m_HorizontalBlurShader;
		m_HorizontalBlurShader = 0;
	}

	// Release the horizontal blur render to texture object.
	if(m_HorizontalBlurTexture)
	{
		m_HorizontalBlurTexture->Shutdown();
		delete m_HorizontalBlurTexture;
		m_HorizontalBlurTexture = 0;
	}

	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the small ortho window object.
	if(m_SmallWindow)
	{
		m_SmallWindow->Shutdown();
		delete m_SmallWindow;
		m_SmallWindow = 0;
	}

	// Release the down sample render to texture object.
	if(m_DownSampleTexure)
	{
		m_DownSampleTexure->Shutdown();
		delete m_DownSampleTexure;
		m_DownSampleTexure = 0;
	}

	// Release the shadow shader object.
	if(m_ShadowShader)
	{
		m_ShadowShader->Shutdown();
		delete m_ShadowShader;
		m_ShadowShader = 0;
	}

	// Release the black and white render to texture.
	if(m_BlackWhiteRenderTexture)
	{
		m_BlackWhiteRenderTexture->Shutdown();
		delete m_BlackWhiteRenderTexture; 
		m_BlackWhiteRenderTexture = 0;
	}

	// Release the depth shader object.
	if(m_DepthShader)
	{
		m_DepthShader->Shutdown();
		delete m_DepthShader;
		m_DepthShader = 0;
	}

	// Release the render to texture object.
	if(m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the ground model object.
	if(m_GroundModel)
	{
		m_GroundModel->Shutdown();
		delete m_GroundModel;
		m_GroundModel = 0;
	}

	// Release the sphere model object.
	if(m_SphereModel)
	{
		m_SphereModel->Shutdown();
		delete m_SphereModel;
		m_SphereModel = 0;
	}

	// Release the cube model object.
	if(m_CubeModel)
	{
		m_CubeModel->Shutdown();
		delete m_CubeModel;
		m_CubeModel = 0;
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


bool GraphicsClass::Frame(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	bool result;
	static float lightPositionX = -5.0f;


	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	// Update the position of the light each frame.
	lightPositionX += 0.05f;
	if(lightPositionX > 5.0f)
	{
		lightPositionX = -5.0f;
	}

	// Update the position of the light.
	m_Light->SetPosition(lightPositionX, 8.0f, -5.0f);

	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::RenderSceneToTexture()
{
	D3DXMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix, translateMatrix;
	float posX, posY, posZ;
	bool result;


	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the light view matrix based on the light's position.
	m_Light->GenerateViewMatrix();

	// Get the world matrix from the d3d object.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Get the view and orthographic matrices from the light object.
	m_Light->GetViewMatrix(lightViewMatrix);
	m_Light->GetProjectionMatrix(lightProjectionMatrix);

	// Setup the translation matrix for the cube model.
	m_CubeModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the cube model with the depth shader.
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	result = m_DepthShader->Render(m_D3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the sphere model.
	m_SphereModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the sphere model with the depth shader.
	m_SphereModel->Render(m_D3D->GetDeviceContext());
	result = m_DepthShader->Render(m_D3D->GetDeviceContext(), m_SphereModel->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the ground model.
	m_GroundModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the ground model with the depth shader.
	m_GroundModel->Render(m_D3D->GetDeviceContext());
	result = m_DepthShader->Render(m_D3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if(!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();

	return true;
}


bool GraphicsClass::RenderBlackAndWhiteShadows()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix;
	D3DXMATRIX lightViewMatrix, lightProjectionMatrix;
	float posX, posY, posZ;
	bool result;


	// Set the render target to be the render to texture.
	m_BlackWhiteRenderTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the render to texture.
	m_BlackWhiteRenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Generate the light view matrix based on the light's position.
	m_Light->GenerateViewMatrix();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Get the light's view and projection matrices from the light object.
	m_Light->GetViewMatrix(lightViewMatrix);
	m_Light->GetProjectionMatrix(lightProjectionMatrix);

	// Setup the translation matrix for the cube model.
	m_CubeModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);
	
	// Render the cube model using the shadow shader.
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	result = m_ShadowShader->Render(m_D3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
									lightProjectionMatrix, m_RenderTexture->GetShaderResourceView(), m_Light->GetPosition());
	if(!result)
	{
		return false;
	}
	
	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the sphere model.
	m_SphereModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the sphere model using the shadow shader.
	m_SphereModel->Render(m_D3D->GetDeviceContext());
	result = m_ShadowShader->Render(m_D3D->GetDeviceContext(), m_SphereModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, 
									lightProjectionMatrix, m_RenderTexture->GetShaderResourceView(), m_Light->GetPosition());
	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the ground model.
	m_GroundModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the ground model using the shadow shader.
	m_GroundModel->Render(m_D3D->GetDeviceContext());
	result = m_ShadowShader->Render(m_D3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
									lightProjectionMatrix, m_RenderTexture->GetShaderResourceView(), m_Light->GetPosition());
	if(!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();

	return true;
}


bool GraphicsClass::DownSampleTexture()
{
	D3DXMATRIX worldMatrix, baseViewMatrix, orthoMatrix;
	bool result;


	// Set the render target to be the render to texture.
	m_DownSampleTexure->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the render to texture.
	m_DownSampleTexure->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	
	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	m_DownSampleTexure->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_D3D->GetDeviceContext());

	// Render the small ortho window using the texture shader and the render to texture of the scene as the texture resource.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, baseViewMatrix, orthoMatrix, 
									 m_BlackWhiteRenderTexture->GetShaderResourceView());
	if(!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();

	return true;
}


bool GraphicsClass::RenderHorizontalBlurToTexture()
{
	D3DXMATRIX worldMatrix, baseViewMatrix, orthoMatrix;
	float screenSizeX;
	bool result;


	// Store the screen width in a float that will be used in the horizontal blur shader.
	screenSizeX = (float)(SHADOWMAP_WIDTH / 2);
	
	// Set the render target to be the render to texture.
	m_HorizontalBlurTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the render to texture.
	m_HorizontalBlurTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_HorizontalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_D3D->GetDeviceContext());
	
	// Render the small ortho window using the horizontal blur shader and the down sampled render to texture resource.
	result = m_HorizontalBlurShader->Render(m_D3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, baseViewMatrix, orthoMatrix,
											m_DownSampleTexure->GetShaderResourceView(), screenSizeX);
	if(!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();
	
	return true;
}


bool GraphicsClass::RenderVerticalBlurToTexture()
{
	D3DXMATRIX worldMatrix, baseViewMatrix, orthoMatrix;
	float screenSizeY;
	bool result;


	// Store the screen height in a float that will be used in the vertical blur shader.
	screenSizeY = (float)(SHADOWMAP_HEIGHT / 2);

	// Set the render target to be the render to texture.
	m_VerticalBlurTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the render to texture.
	m_VerticalBlurTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_VerticalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_D3D->GetDeviceContext());
	
	// Render the small ortho window using the vertical blur shader and the horizontal blurred render to texture resource.
	result = m_VerticalBlurShader->Render(m_D3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, baseViewMatrix, orthoMatrix,
										  m_HorizontalBlurTexture->GetShaderResourceView(), screenSizeY);
	if(!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();
	
	return true;
}


bool GraphicsClass::UpSampleTexture()
{
	D3DXMATRIX worldMatrix, baseViewMatrix, orthoMatrix;
	bool result;


	// Set the render target to be the render to texture.
	m_UpSampleTexure->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the render to texture.
	m_UpSampleTexure->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_UpSampleTexure->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_D3D->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the small sized final blurred render to texture resource.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, baseViewMatrix, orthoMatrix,
									 m_VerticalBlurTexture->GetShaderResourceView());
	if(!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();

	return true;
}


bool GraphicsClass::Render()
{
	bool result;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix;
	float posX, posY, posZ;


	// First render the scene to a texture.
	result = RenderSceneToTexture();
	if(!result)
	{
		return false;
	}

	// Next render the shadowed scene in black and white.
	result = RenderBlackAndWhiteShadows();
	if(!result)
	{
		return false;
	}

	// Then down sample the black and white scene texture.
	result = DownSampleTexture();
	if(!result)
	{
		return false;
	}

	// Perform a horizontal blur on the down sampled texture.
	result = RenderHorizontalBlurToTexture();
	if(!result)
	{
		return false;
	}

	// Now perform a vertical blur on the texture.
	result = RenderVerticalBlurToTexture();
	if(!result)
	{
		return false;
	}

	// Finally up sample the final blurred render to texture that can now be used in the soft shadow shader.
	result = UpSampleTexture();
	if(!result)
	{
		return false;
	}

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Setup the translation matrix for the cube model.
	m_CubeModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);
	
	// Render the cube model using the soft shadow shader.
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	result = m_SoftShadowShader->Render(m_D3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
										m_CubeModel->GetTexture(), m_UpSampleTexure->GetShaderResourceView(), m_Light->GetPosition(), 
										m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the sphere model.
	m_SphereModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the sphere model using the soft shadow shader.
	m_SphereModel->Render(m_D3D->GetDeviceContext());
	result = m_SoftShadowShader->Render(m_D3D->GetDeviceContext(), m_SphereModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
										m_SphereModel->GetTexture(), m_UpSampleTexure->GetShaderResourceView(), m_Light->GetPosition(), 
										m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the ground model.
	m_GroundModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the ground model using the soft shadow shader.
	m_GroundModel->Render(m_D3D->GetDeviceContext());
	result = m_SoftShadowShader->Render(m_D3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
										m_GroundModel->GetTexture(), m_UpSampleTexure->GetShaderResourceView(), m_Light->GetPosition(),
										m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	if(!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}