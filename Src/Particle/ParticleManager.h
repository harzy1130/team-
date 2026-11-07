#pragma once
#include"Constants/Constants.h"
#include"Map/Map.h"
#include"Player/PlayerBase.h"
#include"Camera/Camera.h"
#include<algorithm>

class C_Map;
class C_PlayerBase;
class C_Camera;
class C_ParticleManager
{
public:
	C_ParticleManager();
	~C_ParticleManager() {};

	void Init(C_Map* a_pMap, C_PlayerBase* a_pPlayer, C_Camera* a_pCamera);
	void Update();
	void Draw();
	
	void SetTexture(KdTexture* a_pTex) { m_pChipTex = a_pTex; }

private:
	C_Map* m_pMap;
	C_PlayerBase* m_pPlayer;
	C_Camera* m_pCamera;

	KdTexture* m_pChipTex;
	std::vector<S_ParticleObjects> m_grain;
	float m_spawnIntervalTimer;

	void UpdatePlayer();
	void DrawPlayer();
	int GetUnderChip(Math::Vector2 a_pos);

	float Rnd();
	void EraseGrain(std::vector<S_ParticleObjects>& a_grain);
};