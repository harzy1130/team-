#include "Pch.h"
#include "Enemy1.h"
#include "Map/MapConstants.h"

// Enemy1 の初期ステータスと巡回範囲を設定し、Stage1 左側の平地に足元を合わせる。
bool Enemy1::Init()
{
    // Stage1's left flat floor occupies row MapHeight - 2.
    // Map::Create uses tile CENTERS: LeftEdge + col*TexSize, TopEdge - row*TexSize.
    const float startX = LeftEdge + 2.5f * TexSize;
    const float leftX = LeftEdge + TexSize + TexSize / 4.0f;
    const float rightX = LeftEdge + 6.0f * TexSize - TexSize / 2.0f;
    if (!EnemyBase::Init(startX, 0.0f, leftX, rightX))
    {
        OutputDebugStringA("Enemy1 texture load failed: Texture/Chara.png\n");
        return false;
    }
    const float floorTop = TopEdge - (MapHeight - 2) * TexSize + TexSize / 2.0f;
    SetPos(startX, floorTop - static_cast<float>(GetRect().bottom));
    SetHp(100);
    SetAtk(10);
    SetMoveSpeed(2.0f);
    m_testHitWasDown = false;
    CalcMat();
    return true;
}

// ゲーム画面が選択されている間、H キーを押した瞬間に被ダメージをテストし、敵の共通更新を呼び出す。
void Enemy1::Update()
{
    // Temporary enemy-only test. Real attacks should call TakeDamage on a hit.
    const bool focused = GetActiveWindow() != nullptr
        && GetForegroundWindow() == GetActiveWindow();
    const bool hitDown = focused && (GetAsyncKeyState('H') & 0x8000) != 0;
    if (hitDown && !m_testHitWasDown)
        TakeDamage(25, GetPos().x - 100.0f);
    m_testHitWasDown = hitDown;
    EnemyBase::Update();
}
