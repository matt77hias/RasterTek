////////////////////////////////////////////////////////////////////////////////
// Filename: rendertextureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "rendertextureclass.h"


RenderTextureClass::RenderTextureClass()
{
	m_renderTargetTexture = 0;
	m_renderTargetView = 0;
	m_shaderResourceView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilView = 0;
}


RenderTextureClass::RenderTextureClass(const RenderTextureClass& other)
{
}


RenderTextureClass::~RenderTextureClass()
{
}


bool RenderTextureClass::Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;


	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

	// Create the render target texture.
	result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = device->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the viewport for rendering.
    m_viewport.Width = (float)textureWidth;
    m_viewport.Height = (float)textureHeight;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;

	// Setup the projection matrix.
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, ((float)D3DX_PI / 4.0f), ((float)textureWidth / (float)textureHeight), screenNear, screenDepth);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)textureWidth, (float)textureHeight, screenNear, screenDepth);

	return true;
}


void RenderTextureClass::Shutdown()
{
	if(m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if(m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if(m_shaderResourceView)
	{
		m_shaderResourceView->Release();
		m_shaderResourceView = 0;
	}

	if(m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if(m_renderTargetTexture)
	{
		m_renderTargetTexture->Release();
		m_renderTargetTexture = 0;
	}

	return;
}


void RenderTextureClass::SetRenderTarget(ID3D11DeviceContext* deviceContext)
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	
	// Set the viewport.
	deviceContext->RSSetViewports(1, &m_viewport);
	
	return;
}


void RenderTextureClass::ClearRenderTarget(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha)
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(m_renderTargetView, color);
    
	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


ID3D11ShaderResourceView* RenderTextureClass::GetShaderResourceView()
{
	return m_shaderResourceView;
}


void RenderTextureClass::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}


void RenderTextureClass::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}