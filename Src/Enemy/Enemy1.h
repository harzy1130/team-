#pragma once
#include "EnemyBase.h"

// Enemy1 owns this enemy type's initial settings.
// Movement, drawing, collision data and getters/setters are inherited.
class Enemy1 : public EnemyBase
{
public:
    
    bool Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Map* map = nullptr);
    
    void Update();
    // Scene supplies activity and the live target; stage bookkeeping stays here.
    void Update(const HitBase* target, bool active);
private:
    C_Map* m_map = nullptr;
    bool m_wasActive = false;
    int m_stage = -1;
    bool m_testHitWasDown = false;
};
