#pragma once
#include "EnemyBase.h"

// Enemy1 owns this enemy type's initial settings.
// Movement, drawing, collision data and getters/setters are inherited.
class Enemy1 : public EnemyBase
{
public:
    
    bool Init(C_Hit* a_pHit, C_Camera* a_pCamera);
    
    void Update();
private:
    bool m_testHitWasDown = false;
};
