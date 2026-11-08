#include "Pch.h"
#include "Enemy1.h"
#include "Hit/Hit.h"
#include "Map/Map.h"
#include "Player/PlayerBase.h"
#include <cmath>
#include "Constants/Constants.h"

namespace
{
    constexpr int SpriteWidth = 96;
    constexpr int SpriteHeight = 80;
    constexpr int FootPadding = 4;
    constexpr int NormalWindupFrames = 12;
    constexpr int NormalStrikeFrames = 4;
    constexpr int NormalRecoveryFrames = 14;
    constexpr int NormalCooldownFrames = 30;
    constexpr float NormalReach = 12.0f;

    struct SpritePixels
    {
        int width = 0, height = 0;
        int left = 0, top = 0, right = 0, bottom = 0;
        std::vector<unsigned char> rgba;
    };

    // 透明な生成ノイズを除去し、各コマの本体だけを抽出する。
    bool ReadSpriteFrames(const wchar_t* path, std::vector<SpritePixels>& frames)
    {
        DirectX::ScratchImage original, converted;
        if (FAILED(DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS_NONE, nullptr, original))) return false;
        const DirectX::Image* image = original.GetImage(0, 0, 0);
        if (image == nullptr) return false;
        if (image->format != DXGI_FORMAT_R8G8B8A8_UNORM)
        {
            if (FAILED(DirectX::Convert(*image, DXGI_FORMAT_R8G8B8A8_UNORM,
                DirectX::TEX_FILTER_DEFAULT, 0.0f, converted))) return false;
            image = converted.GetImage(0, 0, 0);
        }
        if (image == nullptr || image->width < 2 || image->height < 2
            || image->width > 4096 || image->height > 4096
            || image->width % 2 != 0 || image->height % 2 != 0) return false;
        const int width = static_cast<int>(image->width / 2);
        const int height = static_cast<int>(image->height / 2);
        for (int cell = 0; cell < 4; ++cell)
        {
            SpritePixels frame;
            frame.width = width; frame.height = height;
            frame.left = width; frame.top = height;
            const int originX = (cell % 2) * width;
            const int originY = (cell / 2) * height;
            std::vector<unsigned char> mask(width * height, 0);
            for (int y = 0; y < height; ++y)
                for (int x = 0; x < width; ++x)
                    mask[y * width + x] = image->pixels[(originY + y) * image->rowPitch
                        + (originX + x) * 4 + 3] >= 128 ? 1 : 0;

            // 最大の連結領域を残し、背景に浮いた点を描画しない。
            std::vector<int> largest, pending;
            for (int start = 0; start < width * height; ++start)
            {
                if (!mask[start]) continue;
                pending.clear(); pending.push_back(start); mask[start] = 0;
                for (std::size_t next = 0; next < pending.size(); ++next)
                {
                    const int at = pending[next], x = at % width, y = at / width;
                    const int neighbours[] = { x > 0 ? at - 1 : -1, x + 1 < width ? at + 1 : -1,
                        y > 0 ? at - width : -1, y + 1 < height ? at + width : -1 };
                    for (const int index : neighbours)
                        if (index >= 0 && mask[index]) { mask[index] = 0; pending.push_back(index); }
                }
                if (pending.size() > largest.size()) largest = pending;
            }
            if (largest.empty()) return false;
            frame.rgba.resize(width * height * 4, 0);
            for (const int at : largest)
            {
                const int x = at % width, y = at / width;
                const unsigned char* pixel = image->pixels + (originY + y) * image->rowPitch + (originX + x) * 4;
                std::copy(pixel, pixel + 3, frame.rgba.begin() + at * 4);
                frame.rgba[at * 4 + 3] = 255;
                frame.left = std::min(frame.left, x); frame.top = std::min(frame.top, y);
                frame.right = std::max(frame.right, x + 1); frame.bottom = std::max(frame.bottom, y + 1);
            }
            frames.push_back(std::move(frame));
        }
        return true;
    }

    // 歩行と突進の全コマを同じ倍率・足元で配置し、最近傍で縮小する。
    bool BuildEnemy1Atlas(std::vector<unsigned char>& pixels)
    {
        std::vector<SpritePixels> frames;
        if (!ReadSpriteFrames(L"Src/Enemy/Texture/Enemy1_Walk.png", frames)
            || !ReadSpriteFrames(L"Src/Enemy/Texture/Enemy1_Dash.png", frames)) return false;
        int maxWidth = 1, maxHeight = 1;
        for (const auto& frame : frames)
        {
            maxWidth = std::max(maxWidth, frame.right - frame.left);
            maxHeight = std::max(maxHeight, frame.bottom - frame.top);
        }
        const float scale = std::min(static_cast<float>(SpriteWidth - 8) / maxWidth,
            static_cast<float>(SpriteHeight - 8) / maxHeight);
        const int atlasWidth = SpriteWidth * 4, atlasHeight = SpriteHeight * 2;
        pixels.assign(atlasWidth * atlasHeight * 4, 0);
        for (std::size_t cell = 0; cell < frames.size(); ++cell)
        {
            const auto& frame = frames[cell];
            const int sourceWidth = frame.right - frame.left, sourceHeight = frame.bottom - frame.top;
            const int width = std::max(1, static_cast<int>(std::round(sourceWidth * scale)));
            const int height = std::max(1, static_cast<int>(std::round(sourceHeight * scale)));
            const int originX = static_cast<int>(cell % 4) * SpriteWidth + (SpriteWidth - width) / 2;
            const int originY = static_cast<int>(cell / 4) * SpriteHeight + SpriteHeight - FootPadding - height;
            for (int y = 0; y < height; ++y)
                for (int x = 0; x < width; ++x)
                {
                    const int sourceX = frame.left + x * sourceWidth / width;
                    const int sourceY = frame.top + y * sourceHeight / height;
                    const auto first = frame.rgba.begin() + (sourceY * frame.width + sourceX) * 4;
                    std::copy(first, first + 4, pixels.begin() + ((originY + y) * atlasWidth + originX + x) * 4);
                }
        }
        return true;
    }

    // CPU 側の完成済みアトラスを共有し、個体の解放では破棄しない。
    const std::vector<unsigned char>& CachedEnemy1Atlas()
    {
        static std::vector<unsigned char> cached;
        if (cached.empty())
        {
            std::vector<unsigned char> prepared;
            if (BuildEnemy1Atlas(prepared)) cached.swap(prepared);
        }
        return cached;
    }

    bool LoadEnemy1Atlas(KdTexture& texture)
    {
        const auto& pixels = CachedEnemy1Atlas();
        if (pixels.empty()) return false;
        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = pixels.data(); data.SysMemPitch = SpriteWidth * 4 * 4;
        return texture.Create(SpriteWidth * 4, SpriteHeight * 2,
            DXGI_FORMAT_R8G8B8A8_UNORM, 1, &data);
    }
}

bool Enemy1::PreloadSprites()
{
    return !CachedEnemy1Atlas().empty();
}


// 種類ごとの既定値を使う初期化。配置データを渡す場合は下のオーバーロードを使う。
bool Enemy1::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Map* map)
{
    return Init(a_pHit, a_pCamera, map, SpawnSettings{});
}

bool Enemy1::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Map* map, const SpawnSettings& spawn)
{
    if (!EnemyBase::Init(a_pHit, a_pCamera, spawn.x, spawn.y, spawn.leftX, spawn.rightX, 2.0f, "Texture/Chara.png"))
    {

        OutputDebugStringA("Enemy1 initialization texture load failed: Texture/Chara.png\n");
        return false;
    }
    // 元画像やアトラスの大きさに関係なく、既存の 64x64 の当たり判定を保つ。
    SetRect(PTEX_RADIUS, PTEX_RADIUS);
    // チーム版の複合当たり判定にも、既存の 64x64 の本体を登録する。
    NewRectObjects({ 0.0f, 0.0f }, GetRect());
    m_hitRange = std::sqrt(2.0f) * PTEX_RADIUS;
    ResetHitRect();
    if (!LoadEnemy1Atlas(GetSpriteTexture()))
    {
        OutputDebugStringA("Enemy1 animation atlas load failed.\n");
        Release();
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
    const bool focused = GetActiveWindow() != nullptr
        && GetForegroundWindow() == GetActiveWindow();
    const bool hitDown = focused && (GetAsyncKeyState('H') & 0x8000) != 0;
    if (m_damageTestEnabled && hitDown && !m_testHitWasDown)
        TakeDamage(25, GetPos().x - 100.0f);
    m_testHitWasDown = hitDown;
    if (GetAlive() && m_normalCooldown > 0) --m_normalCooldown;
    if (!GetAlive() || IsHurt())
    {
        m_openingSkill.Cancel();
        CancelNormalAttack();
    }
    const auto previousPos = GetPos();
    EnemyBase::Update();

    if (!IsChasing() || m_player == nullptr || !m_player->GetAFlg())
    {
        m_openingSkill.ResetEncounter();
        CancelNormalAttack();
    }
    else if (GetAlive() && !IsHurt())
    {
        if (m_openingSkill.IsDashing())
        {
            if (m_openingSkill.TryHit(*this, previousPos, *m_player))
                m_player->TakeDamage(GetAtk() * m_openingSkill.GetDamageMultiplier());
            if (std::abs(GetPos().x - previousPos.x) + 0.01f < m_openingSkill.GetSpeed())
                m_openingSkill.StopAtWall();
        }
        // 一度の通常攻撃では一度だけ命中を調べる。離脱・跳躍・背後への回避が可能。
        if (m_normalHitPending && m_pHit != nullptr
            && m_pHit->CheckAttackHit(*this, *m_player, NormalReach, m_normalDirection))
            m_player->TakeDamage(static_cast<float>(GetAtk()));
    }
    m_normalHitPending = false;
    UpdateAnimation(previousPos);
}


void Enemy1::Update(C_PlayerBase* target, bool active)
{
    if (!active)
    {
        m_wasActive = false;
        ResetAwareness();
        m_openingSkill.ResetEncounter();
        CancelNormalAttack();
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
    // 開幕の突進を優先し、その後は近距離の通常攻撃へ切り替える。
    if (m_openingSkill.Update(*this, m_player, IsChasing(), IsGrounded(), horizontalMove))
    {
        CancelNormalAttack();
        FaceDirection(m_openingSkill.GetDirection());
        return true;
    }
    if (m_player == nullptr || !m_player->GetAFlg() || !IsChasing())
    {
        CancelNormalAttack();
        return false;
    }
    if (m_normalState == NormalState::Ready)
    {
        if (m_normalCooldown > 0 || !IsGrounded() || !InNormalAttackRange()) return false;
        m_normalDirection = m_player->GetPos().x < GetPos().x ? -1 : 1;
        FaceDirection(m_normalDirection);
        m_normalState = NormalState::Windup;
        m_normalFrames = NormalWindupFrames;
    }
    horizontalMove = 0.0f;
    if (--m_normalFrames <= 0)
    {
        if (m_normalState == NormalState::Windup)
        {
            m_normalState = NormalState::Strike;
            m_normalFrames = NormalStrikeFrames;
            m_normalHitPending = true;
        }
        else if (m_normalState == NormalState::Strike)
        {
            m_normalState = NormalState::Recovery;
            m_normalFrames = NormalRecoveryFrames;
        }
        else
        {
            m_normalState = NormalState::Ready;
            m_normalCooldown = NormalCooldownFrames;
        }
    }
    return true;
}

bool Enemy1::InNormalAttackRange() const
{
    return m_player != nullptr && m_player->GetAFlg() && m_pHit != nullptr
        && m_pHit->CheckAttackHit(*this, *m_player, NormalReach);
}

void Enemy1::CancelNormalAttack()
{
    if (m_normalState != NormalState::Ready)
        m_normalCooldown = std::max(m_normalCooldown, NormalCooldownFrames);
    m_normalState = NormalState::Ready;
    m_normalFrames = 0;
    m_normalHitPending = false;
}

void Enemy1::UpdateAnimation(const Math::Vector2& previousPos)
{
    const bool windup = m_openingSkill.IsWindingUp() || m_normalState == NormalState::Windup;
    const bool charge = m_openingSkill.IsDashing() || m_normalState == NormalState::Strike;
    const bool action = windup || charge || m_normalState == NormalState::Recovery;
    if (action != m_wasVisualAction || windup != m_wasVisualWindup) m_animationTicks = 0;
    if (m_wasVisualAction && !action) m_visualRecovery = 8;
    m_wasVisualAction = action;
    m_wasVisualWindup = windup;
    if (!GetAlive() || IsHurt())
    {
        m_spriteFrame = 0; m_animationTicks = 0; m_visualRecovery = 0;
    }
    else if (windup) m_spriteFrame = 4;
    else if (charge)
        m_spriteFrame = m_normalState == NormalState::Strike ? 6 : 5 + (m_animationTicks++ / 4) % 2;
    else if (m_normalState == NormalState::Recovery || m_visualRecovery > 0)
    {
        m_spriteFrame = 7;
        if (m_visualRecovery > 0) --m_visualRecovery;
    }
    else if (std::abs(GetPos().x - previousPos.x) > 0.01f && IsGrounded())
        m_spriteFrame = (m_animationTicks++ / 8) % 4;
    else { m_spriteFrame = 0; m_animationTicks = 0; }
}

void Enemy1::Draw()
{
    const Math::Rectangle source((m_spriteFrame % 4) * SpriteWidth,
        (m_spriteFrame / 4) * SpriteHeight, SpriteWidth, SpriteHeight);
    DrawSpriteFrame(source, SpriteWidth, SpriteHeight, true, FootPadding);
}

void Enemy1::Release()
{
    m_openingSkill.ResetEncounter();
    m_normalState = NormalState::Ready;
    m_normalFrames = 0; m_normalCooldown = 0; m_normalHitPending = false;
    m_spriteFrame = 0; m_animationTicks = 0; m_visualRecovery = 0;
    m_wasVisualAction = false; m_wasVisualWindup = false;
    m_player = nullptr; m_map = nullptr; m_wasActive = false; m_stage = -1;
    EnemyBase::Release();
}
