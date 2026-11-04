#include "Pch.h"
#include "Enemy2.h"
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

    struct SpritePixels
    {
        int width = 0, height = 0;
        int left = 0, top = 0, right = 0, bottom = 0;
        std::vector<unsigned char> rgba;
    };

    // 透明な生成ノイズを除去し、各コマの本体だけを抽出する。
    bool ReadSpriteFrames(const wchar_t* path, std::vector<SpritePixels>& frames, int columns = 2, int rows = 2)
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
            || image->width % columns != 0 || image->height % rows != 0) return false;
        const int width = static_cast<int>(image->width / columns);
        const int height = static_cast<int>(image->height / rows);
        for (int cell = 0; cell < columns * rows; ++cell)
        {
            SpritePixels frame;
            frame.width = width; frame.height = height;
            frame.left = width; frame.top = height;
            const int originX = (cell % columns) * width;
            const int originY = (cell / columns) * height;
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
    bool BuildEnemy2Atlas(std::vector<unsigned char>& pixels)
    {
        std::vector<SpritePixels> frames;
        if (!ReadSpriteFrames(L"Src/Enemy/Texture/Enemy2_Walk.png", frames)
            || !ReadSpriteFrames(L"Src/Enemy/Texture/Enemy2_Shoot.png", frames)) return false;
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
    const std::vector<unsigned char>& CachedEnemy2Atlas()
    {
        static std::vector<unsigned char> cached;
        if (cached.empty())
        {
            std::vector<unsigned char> prepared;
            if (BuildEnemy2Atlas(prepared)) cached.swap(prepared);
        }
        return cached;
    }

    bool LoadEnemy2Atlas(KdTexture& texture)
    {
        const auto& pixels = CachedEnemy2Atlas();
        if (pixels.empty()) return false;
        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = pixels.data(); data.SysMemPitch = SpriteWidth * 4 * 4;
        return texture.Create(SpriteWidth * 4, SpriteHeight * 2,
            DXGI_FORMAT_R8G8B8A8_UNORM, 1, &data);
    }
    // 毒針も最初に一度だけ抽出し、32x12 の描画用データを保持する。
    const std::vector<unsigned char>& CachedNeedlePixels()
    {
        static std::vector<unsigned char> cached;
        if (!cached.empty()) return cached;
        std::vector<SpritePixels> frames;
        if (!ReadSpriteFrames(L"Src/Enemy/Texture/Enemy2_Needle.png", frames, 1, 1)) return cached;
        const auto& frame = frames.front();
        const int sourceWidth = frame.right - frame.left, sourceHeight = frame.bottom - frame.top;
        const float scale = std::min(30.0f / sourceWidth, 10.0f / sourceHeight);
        const int width = std::max(1, static_cast<int>(std::round(sourceWidth * scale)));
        const int height = std::max(1, static_cast<int>(std::round(sourceHeight * scale)));
        cached.assign(32 * 12 * 4, 0);
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
            {
                const int sourceX = frame.left + x * sourceWidth / width;
                const int sourceY = frame.top + y * sourceHeight / height;
                const auto first = frame.rgba.begin() + (sourceY * frame.width + sourceX) * 4;
                const int at = (((12 - height) / 2 + y) * 32 + (32 - width) / 2 + x) * 4;
                std::copy(first, first + 4, cached.begin() + at);
            }
        return cached;
    }

    bool LoadNeedleTexture(KdTexture& texture)
    {
        const auto& pixels = CachedNeedlePixels();
        if (pixels.empty()) return false;
        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = pixels.data(); data.SysMemPitch = 32 * 4;
        return texture.Create(32, 12, DXGI_FORMAT_R8G8B8A8_UNORM, 1, &data);
    }
}

bool Enemy2::PreloadSprites()
{
    return !CachedEnemy2Atlas().empty() && !CachedNeedlePixels().empty();
}

bool Enemy2::Init(C_Hit* hit, C_Camera* camera, C_Map* map)
{
    return Init(hit, camera, map, SpawnSettings{});
}

bool Enemy2::Init(C_Hit* hit, C_Camera* camera, C_Map* map, const SpawnSettings& spawn)
{
    if (!EnemyBase::Init(hit, camera, spawn.x, spawn.y, spawn.leftX, spawn.rightX,
        1.5f, "Texture/Chara.png")) return false;
    SetRect(32, 24);
    NewRectObjects({0.0f, 0.0f}, GetRect());
    m_hitRange = 40.0f;
    if (!LoadEnemy2Atlas(GetSpriteTexture()) || !LoadNeedleTexture(m_needleTexture))
    { Release(); return false; }
    SetHp(80); SetAtk(12); SetMoveSpeed(1.5f); SetChaseSpeed(3.0f);
    SetVision(520.0f, 100.0f, 760.0f); SetVisionMap(map);
    m_map = map;
    m_retreat.Init(C_EnemyBackstepSkill::Settings{});
    SetAttackSettings(AttackSettings{});
    return true;
}

void Enemy2::SetAttackSettings(const AttackSettings& settings)
{
    m_attack = settings;
    m_attack.range = std::isfinite(settings.range) ? std::max(0.0f, settings.range) : 480.0f;
    m_attack.projectileSpeed = std::isfinite(settings.projectileSpeed)
        ? std::max(1.0f, std::min(settings.projectileSpeed, 128.0f)) : 10.0f;
    m_attack.windupFrames = std::max(1, settings.windupFrames);
    m_attack.releaseFrames = std::max(1, settings.releaseFrames);
    m_attack.recoveryFrames = std::max(1, settings.recoveryFrames);
    m_attack.cooldownFrames = std::max(1, settings.cooldownFrames);
    CancelShot(); m_cooldown = 0;
}

void Enemy2::Update(C_PlayerBase* player, bool active)
{
    if (!active)
    {
        ResetAwareness(); m_retreat.ResetEncounter(); CancelShot();
        m_projectiles.clear(); m_player = nullptr; return;
    }
    m_player = player != nullptr && player->GetAFlg() ? player : nullptr;
    SetTarget(m_player);
    const bool focused = GetActiveWindow() != nullptr && GetForegroundWindow() == GetActiveWindow();
    const bool hitDown = focused && (GetAsyncKeyState('H') & 0x8000) != 0;
    if (m_damageTestEnabled && hitDown && !m_testHitWasDown) TakeDamage(25, GetPos().x - 100.0f);
    m_testHitWasDown = hitDown;
    if (!GetAlive()) { m_projectiles.clear(); return; }
    if (m_cooldown > 0) --m_cooldown;
    if (IsHurt()) { m_retreat.Cancel(); CancelShot(); }
    const auto previousPos = GetPos();
    EnemyBase::Update();
    if (!IsChasing() || m_player == nullptr)
    {
        m_retreat.ResetEncounter(); CancelShot(); m_projectiles.clear();
    }
    else
    {
        if (m_firePending && !IsHurt()) FireNeedle();
        m_firePending = false;
        for (auto& projectile : m_projectiles) projectile.Update(m_map, m_player);
        m_projectiles.erase(std::remove_if(m_projectiles.begin(), m_projectiles.end(),
            [](const C_EnemyProjectile& projectile) { return !projectile.GetAlive(); }), m_projectiles.end());
    }
    UpdateAnimation(previousPos);
}

bool Enemy2::GetActionMove(float& horizontalMove)
{
    float jumpSpeed = 0.0f;
    if (m_retreat.Update(*this, m_player, IsChasing(), IsGrounded(), horizontalMove, jumpSpeed))
    {
        CancelShot();
        if (m_player != nullptr) FaceDirection(m_player->GetPos().x < GetPos().x ? -1 : 1);
        if (jumpSpeed > 0.0f) StartJump(jumpSpeed);
        return true;
    }
    if (!IsChasing() || m_player == nullptr) { CancelShot(); return false; }
    if (m_shotState == ShotState::Ready)
    {
        // 射撃間隔中も接近しすぎず、射程外では共通の追跡移動に戻る。
        if (!IsGrounded() || !HasLineOfSight(*m_player, m_attack.range)) return false;
        horizontalMove = 0.0f;
        FaceDirection(m_player->GetPos().x < GetPos().x ? -1 : 1);
        if (m_cooldown > 0) return true;
        m_aimPoint = m_player->GetPos();
        m_shotDirection = m_aimPoint.x < GetPos().x ? -1 : 1;
        m_shotState = ShotState::Windup; m_shotFrames = m_attack.windupFrames;
    }
    horizontalMove = 0.0f;
    FaceDirection(m_shotDirection);
    if (--m_shotFrames <= 0)
    {
        if (m_shotState == ShotState::Windup)
        {
            m_firePending = true;
            m_shotState = ShotState::Release; m_shotFrames = m_attack.releaseFrames;
        }
        else if (m_shotState == ShotState::Release)
        {
            m_shotState = ShotState::Recovery; m_shotFrames = m_attack.recoveryFrames;
        }
        else
        {
            m_shotState = ShotState::Ready; m_cooldown = m_attack.cooldownFrames;
        }
    }
    return true;
}

void Enemy2::CancelShot()
{
    if (m_shotState != ShotState::Ready) m_cooldown = std::max(m_cooldown, 15);
    m_shotState = ShotState::Ready; m_shotFrames = 0; m_firePending = false;
}

void Enemy2::FireNeedle()
{
    if (m_projectiles.size() >= 8) return;
    const auto muzzle = GetPos() + Math::Vector2(m_shotDirection * 40.0f, 24.0f);
    C_EnemyProjectile projectile;
    // 準備開始時の位置へ狙いを固定し、プレイヤーが回避できるようにする。
    if (projectile.Init(muzzle, m_aimPoint - muzzle, m_attack.projectileSpeed, static_cast<float>(GetAtk())))
        m_projectiles.push_back(std::move(projectile));
}

void Enemy2::UpdateAnimation(const Math::Vector2& previousPos)
{
    if (m_shotState == ShotState::Windup)
        m_spriteFrame = m_shotFrames > m_attack.windupFrames / 2 ? 4 : 5;
    else if (m_shotState == ShotState::Release) m_spriteFrame = 6;
    else if (m_shotState == ShotState::Recovery) m_spriteFrame = 7;
    else if (IsRetreating() || std::abs(GetPos().x - previousPos.x) > 0.01f)
        m_spriteFrame = (m_animationTicks++ / 8) % 4;
    else { m_spriteFrame = 0; m_animationTicks = 0; }
}

void Enemy2::Draw()
{
    const Math::Rectangle source((m_spriteFrame % 4) * SpriteWidth,
        (m_spriteFrame / 4) * SpriteHeight, SpriteWidth, SpriteHeight);
    DrawSpriteFrame(source, SpriteWidth, SpriteHeight, true, FootPadding);
    for (const auto& projectile : m_projectiles) projectile.Draw(m_pCamera, &m_needleTexture);
}

void Enemy2::Release()
{
    m_retreat.ResetEncounter(); CancelShot(); m_cooldown = 0;
    m_projectiles.clear(); m_needleTexture.Release(); m_player = nullptr; m_map = nullptr;
    m_spriteFrame = 0; m_animationTicks = 0; m_testHitWasDown = false;
    EnemyBase::Release();
}
