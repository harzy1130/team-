#include "Pch.h"
#include "EnemyBase.h"
#include "Map/Map.h"
#include <cmath>
#include "hit/hit.h"
#include "Constants/Constants.h"

// 敵の座標・描画行列・生存状態を初期化する。
EnemyBase::EnemyBase()
{
    m_pos = Math::Vector2(0.0f, 0.0f);
    m_mat = Math::Matrix::Identity;
    m_bAlive = false;
}

// 初期位置・巡回範囲・速度を設定し、画像と当たり判定を初期化する。画像の読み込み失敗時は false を返す。
bool EnemyBase::Init(C_Hit* a_pHit, C_Camera* a_pCamera, float x, float y, float leftX, float rightX,
    float speed, const char* texturePath)
{
    Release();
    SetPos(x, y);
    m_direction = 1;
    SetPatrolRange(leftX, rightX);
    m_spawnX = m_pos.x;
    m_spawnSearchY = y;
    SetMoveSpeed(speed);
    SetHitClass(a_pHit);
    SetCameraClass(a_pCamera);

    if (texturePath == nullptr || !m_texture.Load(texturePath))
    {
        return false;
    }

    const auto& info = m_texture.GetInfo();
    SetRect(static_cast<long>(info.Width / 2), static_cast<long>(info.Height / 2));
    ResetHitRect();
    m_bAlive = true;
    CalcMat();
    return true;
}

// 左右の巡回範囲を設定し、現在の X 座標をその範囲内に収める。
void EnemyBase::SetPatrolRange(float leftX, float rightX)
{
    m_leftX = std::min(leftX, rightX);
    m_rightX = std::max(leftX, rightX);
    m_pos.x = std::max(m_leftX, std::min(m_pos.x, m_rightX));
}

// HP を 0 以上に設定する。HP が 0 になった場合は死亡状態にして移動を停止する。
void EnemyBase::SetHp(int hp)
{
    m_hp = std::max(0, hp);
    if (m_hp == 0)
    {
        m_bAlive = false;
        m_velocityY = 0.0f;
        SetMove(0.0f, 0.0f);
    }
}

// 有効な攻撃を受けたときに HP を減らし、死亡または攻撃元と反対方向へのノックバックを処理する。
bool EnemyBase::TakeDamage(int damage, float attackFromX)
{
    if (!m_bAlive || m_invincibleFrames > 0 || damage <= 0) return false;

    SetHp(m_hp - std::min(damage, m_hp));
    if (!m_bAlive) return true;

    m_invincibleFrames = 30;
    m_hurtFrames = 12;
    m_knockbackX = (attackFromX <= m_pos.x) ? 8.0f : -8.0f;
    return true;
}

// 攻撃力を 0 以上の値に設定する。
void EnemyBase::SetAtk(int atk)
{
    m_atk = std::max(0, atk);
}

// 1 フレーム当たりの巡回速度を 0 以上の値に設定する。
void EnemyBase::SetMoveSpeed(float speed)
{
    m_moveSpeed = std::max(0.0f, speed);
}

// 無敵時間と被ダメージ状態を更新し、ノックバックまたは左右の巡回移動を行う。
void EnemyBase::Update()
{
    if (!m_bAlive) return;
    if (m_invincibleFrames > 0) --m_invincibleFrames;

    if (m_target == nullptr) m_chasing = false;
    else if (m_chasing)
    {
        const auto delta = m_target->GetPos() - m_pos;
        if (delta.x * delta.x + delta.y * delta.y > m_loseDistance * m_loseDistance)
            m_chasing = false;
    }
    else if (CanSeeTarget()) m_chasing = true;

    const bool hurt = m_hurtFrames > 0;
    float moveX = 0.0f;
    if (hurt)
    {
        --m_hurtFrames;
        moveX = m_knockbackX;
    }
    else if (GetActionMove(moveX))
    {
        // 固有行動は X 方向の移動量だけを指定し、座標への適用は MoveWithCollision で一度だけ行う。
    }
    else if (m_chasing)
    {
        const float dx = m_target->GetPos().x - m_pos.x;
        // ターゲットの X 座標で止め、通り過ぎて毎フレーム向きが反転するのを防ぐ。
        if (std::abs(dx) > 1.0f)
        {
            m_direction = dx > 0.0f ? 1 : -1;
            moveX = static_cast<float>(m_direction) * std::min(m_chaseSpeed, std::abs(dx));
        }
    }
    else if (m_pos.x < m_leftX || m_pos.x > m_rightX)
    {
        // ターゲットを見失った後は、巡回範囲まで歩いて戻る。
        const float goal = m_pos.x < m_leftX ? m_leftX : m_rightX;
        m_direction = goal > m_pos.x ? 1 : -1;
        moveX = static_cast<float>(m_direction) * std::min(m_moveSpeed, std::abs(goal - m_pos.x));
    }
    else
    {
        moveX = m_moveSpeed * static_cast<float>(m_direction);
        const float nextX = m_pos.x + moveX;
        if (nextX >= m_rightX)
        {
            moveX = m_rightX - m_pos.x;
            m_direction = -1;
        }
        else if (nextX <= m_leftX)
        {
            moveX = m_leftX - m_pos.x;
            m_direction = 1;
        }
    }

    const bool wall = MoveWithCollision(moveX);
    if (hurt) m_knockbackX = wall ? 0.0f : m_knockbackX * 0.8f;
    else if (wall && !m_chasing) m_direction = moveX > 0.0f ? -1 : 1;
}

bool EnemyBase::MoveWithCollision(float horizontalMove)
{
    // 落下速度を制限し、1 フレームでマップチップを飛び越えにくくする。
    const float maxFallSpeed = static_cast<float>(CHIP_RADIUS) * 0.5f;
    m_velocityY = std::max(m_velocityY - GRAVITY, -maxFallSpeed);
    SetMove(horizontalMove, m_velocityY);
    ResetHitRect();
    if (m_pHit != nullptr) m_pHit->MapHitCheck(this);

    // ApplyMove が移動量を消す前に、落地・天井衝突で補正された速度を保存する。
    const auto correctedMove = GetMove();
    m_velocityY = correctedMove.y;
    const bool wall = horizontalMove != 0.0f && correctedMove.x == 0.0f;
    ApplyMove();
    return wall;
}

// 移動方向に応じた左右反転と、現在位置への平行移動を描画行列に反映する。
void EnemyBase::CalcMat()
{
    m_draw = m_pCamera ? m_pos - m_pCamera->GetPos() : m_pos;
    // 左向きの場合は Y 軸を中心に 180 度回転し、画像を左右反転する。
    const float angle = (m_direction < 0) ? DirectX::XM_PI : 0.0f;
    m_mat = Math::Matrix::CreateRotationY(angle)
        * Math::Matrix::CreateTranslation(m_draw.x, m_draw.y, 0.0f);
}

// 生存中の敵を描画する。無敵時間中は表示を切り替えて点滅させる。
void EnemyBase::Draw()
{
    if (!m_bAlive) return;
    if (m_invincibleFrames > 0 && (m_invincibleFrames / 4) % 2 == 0) return;

    //// Update後にSetPosが呼ばれた場合に備えて、描画行列を再計算する。
    // カメラスクロールのためいったんコメントアウトしました（吉田）
    CalcMat();
    const auto& info = m_texture.GetInfo();
    SHADER.m_spriteShader.SetMatrix(m_mat);
    SHADER.m_spriteShader.DrawTex(&m_texture,
        Math::Rectangle(0, 0, static_cast<int>(info.Width),
            static_cast<int>(info.Height)), 1.0f);
}


void EnemyBase::Release()
{
    ResetAwareness();
    m_visionMap = nullptr;
    m_bAlive = false;
    m_invincibleFrames = 0;
    m_hurtFrames = 0;
    m_knockbackX = 0.0f;
    m_velocityY = 0.0f;
    SetMove(0.0f, 0.0f);
    m_texture.Release();
}

// 発見距離と追跡解除距離を分け、状態が短時間で切り替わり続けるのを防ぐ。
void EnemyBase::SetVision(float range, float fullAngleDegrees, float loseDistance)
{
    m_visionRange = std::max(1.0f, range);
    m_visionHalfAngle = std::max(1.0f, std::min(fullAngleDegrees, 170.0f)) * DirectX::XM_PI / 360.0f;
    m_loseDistance = std::max(m_visionRange + 1.0f, loseDistance);
}

// チップの中心座標は X: LEFT_EDGE + 列 * CHIP_SIZE、Y: TOP_EDGE - 行 * CHIP_SIZE。
// GetCollisionFlg は負の添字を確認しないため、ここで範囲をチェックする。
float EnemyBase::TraceVision(float directionX, float directionY, float distance) const
{
    if (m_visionMap == nullptr) return distance;
    const int steps = std::max(1, static_cast<int>(std::ceil(distance / 4.0f)));
    float previous = 0.0f;
    for (int i = 0; i <= steps; ++i)
    {
        const float t = distance * static_cast<float>(i) / steps;
        const float x = m_pos.x + directionX * t;
        const float y = m_pos.y + directionY * t;
        const int col = static_cast<int>(std::floor((x - LEFT_EDGE + CHIP_RADIUS) / CHIP_SIZE));
        const int row = static_cast<int>(std::floor((TOP_EDGE - y + CHIP_RADIUS) / CHIP_SIZE));
        if (col < 0 || col >= MAP_WIDTH || row < 0 || row >= MAP_HEIGHT
            || m_visionMap->GetCollisionFlg(row, col)) return previous;
        previous = t;
    }
    return distance;
}

bool EnemyBase::CanSeeTarget() const
{
    if (m_target == nullptr) return false;
    const auto delta = m_target->GetPos() - m_pos;
    const float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    if (distance > m_visionRange) return false;
    if (distance < 0.001f) return true;
    if (delta.x * m_direction < distance * std::cos(m_visionHalfAngle)) return false;
    return TraceVision(delta.x / distance, delta.y / distance, distance) >= distance;
}

// 検知と同じ壁で遮られる扇形の視野を、両キャラクターより奥に描画する。
void EnemyBase::DrawVision()
{
    if (!m_bAlive) return;
    const auto draw = m_pCamera ? m_pos - m_pCamera->GetPos() : m_pos;
    SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(draw.x, draw.y, 0.0f));
    constexpr int segments = 24;
    constexpr int bands = 3;
    Math::Vector2 rays[segments + 1];
    for (int i = 0; i <= segments; ++i)
    {
        const float angle = -m_visionHalfAngle + 2.0f * m_visionHalfAngle * i / segments;
        const float dx = std::cos(angle) * m_direction;
        const float dy = std::sin(angle);
        const float length = TraceVision(dx, dy, m_visionRange);
        rays[i] = Math::Vector2(dx * length, dy * length);
    }
    for (int band = 0; band < bands; ++band)
    {
        const float inner = static_cast<float>(band) / bands;
        const float outer = static_cast<float>(band + 1) / bands;
        const float alpha = 0.20f - band * 0.05f;
        const Math::Color color = m_chasing ? Math::Color(1.0f, 0.25f, 0.12f, alpha)
            : Math::Color(1.0f, 0.9f, 0.3f, alpha);
        for (int i = 0; i < segments; ++i)
        {
            const auto a = rays[i] * inner;
            const auto b = rays[i] * outer;
            const auto c = rays[i + 1] * outer;
            const auto d = rays[i + 1] * inner;
            SHADER.m_spriteShader.DrawTriangle(static_cast<int>(a.x), static_cast<int>(a.y),
                static_cast<int>(b.x), static_cast<int>(b.y), static_cast<int>(c.x), static_cast<int>(c.y), &color);
            if (band > 0)
                SHADER.m_spriteShader.DrawTriangle(static_cast<int>(a.x), static_cast<int>(a.y),
                    static_cast<int>(c.x), static_cast<int>(c.y), static_cast<int>(d.x), static_cast<int>(d.y), &color);
        }
    }
    SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
}
bool EnemyBase::PlaceOnMap(C_Map* map)
{
    if (map == nullptr) return false;
    // チップの中心座標を基準に、中心の 1 マスだけでなく足元の幅全体を調べる。
    const int firstCol = static_cast<int>(std::floor(
        (m_spawnX + m_rect.left - LEFT_EDGE + CHIP_RADIUS + 0.01f) / CHIP_SIZE));
    const int lastCol = static_cast<int>(std::floor(
        (m_spawnX + m_rect.right - LEFT_EDGE + CHIP_RADIUS - 0.01f) / CHIP_SIZE));
    if (firstCol < 0 || lastCol >= MAP_WIDTH) return false;
    for (int row = 0; row < MAP_HEIGHT; ++row)
    {
        const float floorTop = TOP_EDGE - row * CHIP_SIZE + CHIP_RADIUS;
        // 指定した高さより上の足場を選ばず、多段地形の配置にも対応する。
        if (floorTop > m_spawnSearchY + m_rect.bottom) continue;
        bool ground = false;
        for (int col = firstCol; col <= lastCol; ++col)
            if (map->GetCollisionFlg(row, col)) ground = true;
        if (!ground) continue;

        // 開始時の短い落下は残し、実際の地面より上から出現させる。
        SetPos(m_spawnX, floorTop - static_cast<float>(m_rect.bottom) + CHIP_SIZE);
        ResetAwareness();
        m_direction = 1;
        m_velocityY = 0.0f;
        m_knockbackX = 0.0f;
        m_hurtFrames = 0;
        m_invincibleFrames = 0;
        SetMove(0.0f, 0.0f);
        ResetHitRect();
        CalcMat();
        return true;
    }
    return false;
}