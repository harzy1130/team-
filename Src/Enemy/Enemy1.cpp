#include "Pch.h"
#include "Enemy1.h"
#include "Map/Map.h"
#include "Constants/Constants.h"

// Enemy1 の初期ステータスと巡回範囲を設定し、Stage1 左側の平地に足元を合わせる。
bool Enemy1::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Map* map)
{
    // Stage1's left flat floor occupies row MapHeight - 2.
    // Map::Create uses tile CENTERS: LEFT_EDGE + col*PTEX_SIZE, TopEdge - row*PTEX_SIZE.
    const float startX = LEFT_EDGE + 2.5f * PTEX_SIZE;
    const float leftX = LEFT_EDGE + PTEX_SIZE + PTEX_SIZE / 4.0f;
    const float rightX = LEFT_EDGE + 6.0f * PTEX_SIZE - PTEX_SIZE / 2.0f;
    if (!EnemyBase::Init(a_pHit, a_pCamera, startX, 0.0f, leftX, rightX))
    {
        OutputDebugStringA("Enemy1 texture load failed: Texture/Chara.png\n");
        return false;
    }
    const float floorTop = TOP_EDGE - (MAP_HEIGHT - 2) * PTEX_SIZE + PTEX_SIZE / 2.0f;
    // 地面より 1 マップチップ分高い位置に出現させ、落下と着地を確認する。
    SetPos(startX, floorTop - static_cast<float>(GetRect().bottom) + CHIP_SIZE);
    SetHp(100);
    SetAtk(10);
    SetMoveSpeed(2.0f);
    SetVision(320.0f, 60.0f, 560.0f);
    SetChaseSpeed(6.0f);
    SetVisionMap(map);
    m_map = map;
    m_wasActive = false;
    m_stage = -1;

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

void Enemy1::Update(const HitBase* target, bool active)
{
    if (!active)
    {
        m_wasActive = false;
        ResetAwareness();
        return;
    }

    // Covers normal entry, reentry, and switching stages directly from the GUI.
    if (m_map != nullptr && (!m_wasActive || m_stage != m_map->GetStageNum()))
    {
        if (!PlaceOnMap(m_map))
            OutputDebugStringA("Enemy spawn failed: no ground below spawn column.\n");
        m_stage = m_map->GetStageNum();
    }
    m_wasActive = true;
    SetTarget(target);
    Update();
}