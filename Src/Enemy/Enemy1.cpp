#include "Pch.h"
#include "Enemy1.h"

bool Enemy1::Init()
{
    
    if (!EnemyBase::Init(0.0f, 0.0f, -200.0f, 200.0f))
    {
        OutputDebugStringA("Enemy1 texture load failed: Texture/Chara.png\n");
        return false;
    }

    SetHp(100);
    SetAtk(10);
    SetMoveSpeed(2.0f);
    return true;
}
