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

struct S_Config
{
    ParticleType type;
    Math::Vector2 pos;
    Math::Vector2 move;
    std::pair<float, float> randomOffsetX;
    std::pair<float, float> randomOffsetY;
    std::pair<float, float> randomOffsetMoveX;
    std::pair<float, float> randomOffsetMoveY;
    long size;
    float angle;
    float alpha;
    float lifeSpan;
    bool IsRepeat;
    //生成数
    int generateNum;
    //連続生成時間
    int generateTime;
    //生成間隔
    int generateInterval;
    //プレイヤーに追従するか
    bool IsFollowPlayer;
};

struct S_Particle
{
    ParticleType type;
    Math::Vector2 pos;
    Math::Vector2 move;
    long size;
    float angle;
    float alpha;
    int lifeSpan;
    bool IsActive;
    int data;
};

struct S_Emitter
{
    ParticleType type;
    Math::Vector2 pos;
    //経過時間
    int elapsedTime;
    //生成間隔時間
    int generateIntervalTime;
    bool IsActive;
    bool IsRepeat;
    bool IsFollowPlayer;
};

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

    int Getparticle() { return m_particles.size(); }

    void SetTexture(KdTexture* a_pChipTex, KdTexture* a_pGrainTex, KdTexture* a_pAtkUpTex, KdTexture* a_pDefUpTex, KdTexture* a_pSmokeTex);

private:
    C_Map* m_pMap;
    C_PlayerBase* m_pPlayer;
    C_Camera* m_pCamera;
    C_Effect* m_pEffect;

    KdTexture* m_pChipTex;
    KdTexture* m_pGrainTex;
    KdTexture* m_pAtkUpTex;
    KdTexture* m_pDefUpTex;
    KdTexture* m_pSmokeTex;



    std::vector< S_Config> m_config;
    std::vector< S_Emitter> m_emitter;
    std::vector< S_Particle> m_particles;
    std::vector< ParticleType> m_playedTypes;

    void UpdatePlayer();
    void DrawPlayer();

    void startEmitter(ParticleType a_type, Math::Vector2 a_pos);
    void spawnParticle(ParticleType a_type, Math::Vector2 a_pos);
    void UpdateParticle();
    void UpdateEmitter();
    void FinishEmitter(S_Emitter& emitter);
    //Emitterの生成要求を受け取る関数
    void UpdateEmitterRequest(ParticleType a_type, bool IsRequested, Math::Vector2 a_pos);

    void DrawDust(S_Particle particle);
    void DrawAtk(S_Particle particle);

    void DrawPoison(S_Particle particle);
    void DrawPoison2(S_Particle particle);

    const S_Config* GetConfig(ParticleType a_type);
    int GetUnderChip(Math::Vector2 a_pos);
    bool IsPlaying(ParticleType a_type);
    float Rnd();

    void EraseParticle(std::vector<S_Particle>& a_grain);
};