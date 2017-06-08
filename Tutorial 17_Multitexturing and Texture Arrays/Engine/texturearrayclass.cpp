////////////////////////////////////////////////////////////////////////////////
// Filename: texturearrayclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "texturearrayclass.h"


TextureArrayClass::TextureArrayClass()
{
	m_textures[0] = 0;
	m_textures[1] = 0;
}


TextureArrayClass::TextureArrayClass(const TextureArrayClass& other)
{
}


TextureArrayClass::~TextureArrayClass()
{
}


bool TextureArrayClass::Initialize(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2)
{
	HRESULT result;


	// Load the first texture in.
	result = D3DX11CreateShaderResourceViewFromFile(device, filename1, NULL, NULL, &m_textures[0], NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Load the second texture in.
	result = D3DX11CreateShaderResourceViewFromFile(device, filename2, NULL, NULL, &m_textures[1], NULL);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void TextureArrayClass::Shutdown()
{
	// Release the texture resources.
	if(m_textures[0])
	{
		m_textures[0]->Release();
		m_textures[0] = 0;
	}

	if(m_textures[1])
	{
		m_textures[1]->Release();
		m_textures[1] = 0;
	}

	return;
}


ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
	return m_textures;
}