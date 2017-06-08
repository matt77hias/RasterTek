////////////////////////////////////////////////////////////////////////////////
// Filename: treeclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TREECLASS_H_
#define _TREEMODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TreeClass
////////////////////////////////////////////////////////////////////////////////
class TreeClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	TreeClass();
	TreeClass(const TreeClass&);
	~TreeClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, char*, WCHAR*, float);
	void Shutdown();

	void RenderTrunk(ID3D11DeviceContext*);
	void RenderLeaves(ID3D11DeviceContext*);

	int GetTrunkIndexCount();
	int GetLeafIndexCount();
	
	ID3D11ShaderResourceView* GetTrunkTexture();
	ID3D11ShaderResourceView* GetLeafTexture();
	
	void SetPosition(float, float, float);
	void GetPosition(float&, float&, float&);

private:
	bool InitializeTrunkBuffers(ID3D11Device*, float);
	bool InitializeLeafBuffers(ID3D11Device*, float);

	void ShutdownBuffers();

	void RenderTrunkBuffers(ID3D11DeviceContext*);
	void RenderLeafBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*);
	void ReleaseTextures();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Buffer *m_trunkVertexBuffer, *m_trunkIndexBuffer, *m_leafVertexBuffer, *m_leafIndexBuffer;
	int m_vertexCount, m_indexCount, m_trunkIndexCount, m_leafIndexCount;
	TextureClass *m_TrunkTexture, *m_LeafTexture;
	ModelType* m_model;
	float m_positionX, m_positionY, m_positionZ;
};

#endif