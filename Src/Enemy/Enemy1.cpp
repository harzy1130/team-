#include "Pch.h"
#include "Enemy1.h"
#include "Map/Map.h"
#include "Player/PlayerBase.h"
#include <cmath>
#include "Constants/Constants.h"

// 種類ごとの既定値を使う初期化。配置データを渡す場合は下のオーバーロードを使う。
bool Enemy1::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Map* map)
{
    return Init(a_pHit, a_pCamera, map, SpawnSettings{});
}

bool Enemy1::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Map* map, const SpawnSettings& spawn)
{
    if (!EnemyBase::Init(a_pHit, a_pCamera, spawn.x, spawn.y, spawn.leftX, spawn.rightX))
    {
        OutputDebugStringA("Enemy1 texture load failed: Texture/Chara.png\n");
        return false;
    }
    SetHp(100);
    SetAtk(10);
    SetMoveSpeed(2.0f);
    SetVision(320.0f, 60.0f, 560.0f);
    SetChaseSpeed(6.0f);
    SetVisionMap(map);
    m_map = map;
    m_wasActive = false;
    m_stage = -1;

    C_EnemySkill::DashSettings dash;
    dash.speed = 14.0f;
    dash.windupFrames = 24;
    dash.dashFrames = 20;
    dash.activationRange = 320.0f;
    dash.damageMultiplier = 1.0f;
    m_openingSkill.Init(dash);
    m_player = nullptr;
    m_damageTestEnabled = true;
    m_testHitWasDown = false;
    CalcMat();
    return true;
}

// ゲーム画面が選択されている間、H キーを押した瞬間に被ダメージをテストし、敵の共通更新を呼び出す。
void Enemy1::Update()
{
    // 敵の被ダメージ確認用の仮処理。実際の攻撃では命中時に TakeDamage を呼ぶ。
    const bool focused = GetActiveWindow() != nullptr
        && GetForegroundWindow() == GetActiveWindow();
    const bool hitDown = focused && (GetAsyncKeyState('H') & 0x8000) != 0;
    if (m_damageTestEnabled && hitDown && !m_testHitWasDown)
        TakeDamage(25, GetPos().x - 100.0f);
    m_testHitWasDown = hitDown;
    if (!GetAlive() || IsHurt()) m_openingSkill.Cancel();
    const auto previousPos = GetPos();
    EnemyBase::Update();

    if (!IsChasing() || m_player == nullptr || !m_player->GetAFlg())
    {
        m_openingSkill.ResetEncounter();
        return;
    }
    if (m_openingSkill.IsDashing())
    {
        if (m_openingSkill.TryHit(*this, previousPos, *m_player))
            m_player->TakeDamage(GetAtk() * m_openingSkill.GetDamageMultiplier());
        if (std::abs(GetPos().x - previousPos.x) + 0.01f < m_openingSkill.GetSpeed())
            m_openingSkill.StopAtWall();
    }
}

void Enemy1::Update(C_PlayerBase* target, bool active)
{
    if (!active)
    {
        m_wasActive = false;
        ResetAwareness();
        m_openingSkill.ResetEncounter();
        m_player = nullptr;
        return;
    }

    // 通常の入場・再入場・GUI からの直接ステージ切り替えに対応する。
    if (m_map != nullptr && (!m_wasActive || m_stage != m_map->GetStageNum()))
    {
        if (!PlaceOnMap(m_map))
            OutputDebugStringA("Enemy spawn failed: no ground below spawn column.\n");
        m_openingSkill.ResetEncounter();
        m_stage = m_map->GetStageNum();
    }
    m_wasActive = true;
    m_player = target != nullptr && target->GetAFlg() ? target : nullptr;
    SetTarget(m_player);
    Update();
}
bool Enemy1::GetActionMove(float& horizontalMove)
{
    const bool action = m_openingSkill.Update(*this, m_player, IsChasing(), IsGrounded(), horizontalMove);
    if (action) FaceDirection(m_openingSkill.GetDirection());
    return action;
}