////////////////////////////////////////////////////////////////////////////////
// Filename: fireshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FIRESHADERCLASS_H_
#define _FIRESHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: FireShaderClass
////////////////////////////////////////////////////////////////////////////////
class FireShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct NoiseBufferType
	{
		float frameTime;
		D3DXVECTOR3 scrollSpeeds;
		D3DXVECTOR3 scales;
		float padding;
	};

	struct DistortionBufferType
	{
		D3DXVECTOR2 distortion1;
		D3DXVECTOR2 distortion2;
		D3DXVECTOR2 distortion3;
		float distortionScale;
		float distortionBias;
	};

public:
	FireShaderClass();
	FireShaderClass(const FireShaderClass&);
	~FireShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, 
				ID3D11ShaderResourceView*, float, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2, D3DXVECTOR2, D3DXVECTOR2, float, float);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, 
							 ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2, 
							 D3DXVECTOR2, D3DXVECTOR2, float, float);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_noiseBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* m_sampleState2;
	ID3D11Buffer* m_distortionBuffer;
};

#endif