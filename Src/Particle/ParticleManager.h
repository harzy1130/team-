#pragma once

#include "Constants/Constants.h"
#include "Map/Map.h"
#include "Player/PlayerBase.h"
#include "Camera/Camera.h"
#include "Effect/effect.h"
#include <algorithm>
#include <vector>

enum ParticleType
{
    dust = 0,
    atkUp = 1,
    defUp1 = 2,
    defUp2 = 3,
    heal = 4,
    poison = 5,
    bleed = 6
};

struct S_ParticleObjects
{
    ParticleType type;
    Math::Vector2 pos;
    Math::Vector2 move;
    Math::Vector2 draw;
    Math::Vector2 scale;
    Math::Color color;
    Math::Rectangle rect;
    Math::Matrix mat;
    Math::Matrix sMat;
    Math::Matrix rMat;
    Math::Matrix tMat;
    long size;
    float angle;
    float alpha;
    int data;
    float lifeSpan;
    float spawnIntervalTime;
    bool IsActive;
};

//struct S_ParticleConfig
//{
//	ParticleType type;
//    Math::Vector2 pos;
//    Math::Vector2 move;
//	int generateTime;//òAë±ê∂ê¨éûä‘
//	int generateInterval;//ê∂ê¨ä‘äu
//	float lifeSpan;
//    long size;
//    float angle;
//};
//
//struct S_Particle
//{
//    ParticleType type;
//    Math::Vector2 pos;
//    Math::Vector2 move;
//    Math::Rectangle rect;
//    Math::Matrix tmat;
//    Math::Matrix smat;
//    Math::Matrix mat;
//	int elapsedTime;//åoâﬂéûä‘
//	int generateIntervalTime;//ê∂ê¨ä‘äuéûä‘
//	int lifeSpan;
//	bool IsActive;
//    int data;
//};

class C_Map;
class C_PlayerBase;
class C_Camera;
class C_Effect;

class C_ParticleManager
{
public:
    C_ParticleManager();
    ~C_ParticleManager() = default;

    void Init(C_Map* a_pMap, C_PlayerBase* a_pPlayer, C_Camera* a_pCamera);
    void Update();
    void Draw();

    void SetTexture(KdTexture* a_pChipTex, KdTexture* a_pGrainTex, KdTexture* a_pAtkUpTex, KdTexture* a_pDefUpTex, KdTexture* a_pSmokeTex);

private:
    C_Map* m_pMap;
    C_PlayerBase* m_pPlayer;
    C_Camera* m_pCamera;
    C_Effect m_pEffect;

    KdTexture* m_pChipTex;
    KdTexture* m_pGrainTex;
    KdTexture* m_pAtkUpTex;
    KdTexture* m_pDefUpTex;
    KdTexture* m_pSmokeTex;


    //std::vector< S_ParticleConfig> m_particleConfig;
    //std::vector< S_Particle> m_particles;



    std::vector<S_ParticleObjects> m_grain;

    float m_spawnIntervalTimer;
    float m_spawnDefIntervalTimer;
    float m_spawnPoisonIntervalTimer;
    bool m_bDefRepeat;
    bool m_bPoisonRepeat;
    float m_defRpeatTime;
    float m_poisonRpeatTime;
    float m_spawnAtkIntervalTimer;
    bool m_bAtkRepeat;
    float m_atkRpeatTime;

    void UpdatePlayer();
    void DrawPlayer();

    void startParticle(ParticleType a_type, Math::Vector2 a_pos);
    void spawnParticle(ParticleType a_type, Math::Vector2 a_pos);
    void UpdateParticle();
    //const S_ParticleConfig* GetConfig(ParticleType a_type);
    int GetUnderChip(Math::Vector2 a_pos);
    float Rnd();
    void EraseGrain(std::vector<S_ParticleObjects>& a_grain);
};