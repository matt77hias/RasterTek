////////////////////////////////////////////////////////////////////////////////
// Filename: particlesystemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PARTICLESYSTEMCLASS_H_
#define _PARTICLESYSTEMCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ParticleSystemClass
////////////////////////////////////////////////////////////////////////////////
class ParticleSystemClass
{
private:
	struct ParticleType
	{
		float positionX, positionY, positionZ;
		float red, green, blue;
		float velocity;
		bool active;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR4 color;
	};

public:
	ParticleSystemClass();
	ParticleSystemClass(const ParticleSystemClass&);
	~ParticleSystemClass();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	bool Frame(float, ID3D11DeviceContext*);
	void Render(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();
	int GetIndexCount();

private:
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool InitializeParticleSystem();
	void ShutdownParticleSystem();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();

	void EmitParticles(float);
	void UpdateParticles(float);
	void KillParticles();

	bool UpdateBuffers(ID3D11DeviceContext*);

	void RenderBuffers(ID3D11DeviceContext*);

private:
	float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
	float m_particleVelocity, m_particleVelocityVariation;
	float m_particleSize, m_particlesPerSecond;
	int m_maxParticles;

	int m_currentParticleCount;
	float m_accumulatedTime;

	TextureClass* m_Texture;
	ParticleType* m_particleList;
	int m_vertexCount, m_indexCount;
	VertexType* m_vertices;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
};

#endif