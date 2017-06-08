////////////////////////////////////////////////////////////////////////////////
// Filename: treeclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "treeclass.h"


TreeClass::TreeClass()
{
	m_trunkVertexBuffer = 0;
	m_trunkIndexBuffer = 0;
	m_leafVertexBuffer = 0;
	m_leafIndexBuffer = 0;
	m_TrunkTexture = 0;
	m_TrunkTexture = 0;
	m_model = 0;
}


TreeClass::TreeClass(const TreeClass& other)
{
}


TreeClass::~TreeClass()
{
}


bool TreeClass::Initialize(ID3D11Device* device, char* trunkModelFilename, WCHAR* trunkTextureFilename, char* leafModelFilename, WCHAR* leafTextureFilename, float scale)
{
	bool result;


	// Load in the tree trunk model data.
	result = LoadModel(trunkModelFilename);
	if(!result)
	{
		return false;
	}

	// Store the trunk index count;
	m_trunkIndexCount = m_indexCount;

	// Initialize the vertex and index buffer that hold the geometry for the tree trunk.
	result = InitializeTrunkBuffers(device, scale);
	if(!result)
	{
		return false;
	}

	// Release the tree trunk model data since it is loaded into the buffers.
	ReleaseModel();

	// Load in the tree leaf model data.
	result = LoadModel(leafModelFilename);
	if(!result)
	{
		return false;
	}

	// Store the leaf index count;
	m_leafIndexCount = m_indexCount;

	// Initialize the vertex and index buffer that hold the geometry for the tree leaves.
	result = InitializeLeafBuffers(device, scale);
	if(!result)
	{
		return false;
	}

	// Release the tree leaf model data.
	ReleaseModel();

	// Load the textures for the tree model.
	result = LoadTextures(device, trunkTextureFilename, leafTextureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void TreeClass::Shutdown()
{
	// Release the textures.
	ReleaseTextures();

	// Release the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data if it hasn't been released yet.
	ReleaseModel();

	return;
}


void TreeClass::RenderTrunk(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderTrunkBuffers(deviceContext);

	return;
}


void TreeClass::RenderLeaves(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderLeafBuffers(deviceContext);

	return;
}


int TreeClass::GetTrunkIndexCount()
{
	return m_trunkIndexCount;
}


int TreeClass::GetLeafIndexCount()
{
	return m_leafIndexCount;
}


ID3D11ShaderResourceView* TreeClass::GetTrunkTexture()
{
	return m_TrunkTexture->GetTexture();
}


ID3D11ShaderResourceView* TreeClass::GetLeafTexture()
{
	return m_LeafTexture->GetTexture();
}


bool TreeClass::InitializeTrunkBuffers(ID3D11Device* device, float scale)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for(i=0; i<m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x * scale, m_model[i].y * scale, m_model[i].z * scale);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_trunkVertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_trunkIndexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


bool TreeClass::InitializeLeafBuffers(ID3D11Device* device, float scale)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for(i=0; i<m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x * scale, m_model[i].y * scale, m_model[i].z * scale);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_leafVertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_leafIndexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void TreeClass::ShutdownBuffers()
{
	// Release the buffers.
	if(m_leafIndexBuffer)
	{
		m_leafIndexBuffer->Release();
		m_leafIndexBuffer = 0;
	}

	if(m_leafVertexBuffer)
	{
		m_leafVertexBuffer->Release();
		m_leafVertexBuffer = 0;
	}

	if(m_trunkIndexBuffer)
	{
		m_trunkIndexBuffer->Release();
		m_trunkIndexBuffer = 0;
	}

	if(m_trunkVertexBuffer)
	{
		m_trunkVertexBuffer->Release();
		m_trunkVertexBuffer = 0;
	}

	return;
}


void TreeClass::RenderTrunkBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_trunkVertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_trunkIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


void TreeClass::RenderLeafBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_leafVertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_leafIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool TreeClass::LoadTextures(ID3D11Device* device, WCHAR* trunkFilename, WCHAR* leafFilename)
{
	bool result;


	// Create the trunk texture object.
	m_TrunkTexture = new TextureClass;
	if(!m_TrunkTexture)
	{
		return false;
	}

	// Initialize the trunk texture object.
	result = m_TrunkTexture->Initialize(device, trunkFilename);
	if(!result)
	{
		return false;
	}

	// Create the leaf texture object.
	m_LeafTexture = new TextureClass;
	if(!m_LeafTexture)
	{
		return false;
	}

	// Initialize the leaf texture object.
	result = m_LeafTexture->Initialize(device, leafFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void TreeClass::ReleaseTextures()
{
	// Release the texture objects.
	if(m_LeafTexture)
	{
		m_LeafTexture->Shutdown();
		delete m_LeafTexture;
		m_LeafTexture = 0;
	}

	if(m_TrunkTexture)
	{
		m_TrunkTexture->Shutdown();
		delete m_TrunkTexture;
		m_TrunkTexture = 0;
	}

	return;
}


bool TreeClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);
	
	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if(!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}


void TreeClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}

	return;
}


void TreeClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void TreeClass::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}