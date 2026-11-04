#pragma once
#include "hit/HitBase.h"

class C_Map;
class C_PlayerBase;

// 直進する敵弾。軌道上で最初に触れた地形またはプレイヤーで消える。
class C_EnemyProjectile : public HitBase
{
public:
    bool Init(const Math::Vector2& position, const Math::Vector2& direction,
        float speed, float damage, int lifetimeFrames = 120);
    void Update(C_Map* map, C_PlayerBase* player);
    void Draw(C_Camera* camera, KdTexture* texture) const;
    bool GetAlive() const { return m_alive; }

private:
    Math::Vector2 m_velocity{0.0f, 0.0f};
    float m_damage = 0.0f;
    float m_angle = 0.0f;
    int m_frames = 0;
    bool m_alive = false;
};
