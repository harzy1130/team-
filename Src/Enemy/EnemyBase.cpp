#include "Pch.h"
#include "EnemyBase.h"

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
    SetMoveSpeed(speed);
    SetHitClass(a_pHit);
    SetCameraClass(a_pCamera);

    if (texturePath == nullptr || !m_texture.Load(texturePath))
    {
        return false;
    }

    const auto& info = m_texture.GetInfo();
    SetRect(static_cast<long>(info.Width / 2), static_cast<long>(info.Height / 2));
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
    if (m_hurtFrames > 0)
    {
        --m_hurtFrames;
        const float nextX = std::max(m_leftX,
            std::min(m_pos.x + m_knockbackX, m_rightX));
        SetMove(nextX - m_pos.x, 0.0f);
        ApplyMove();
        m_knockbackX *= 0.8f;
        return;
    }

    SetMove(m_moveSpeed * static_cast<float>(m_direction), 0.0f);
    const float nextX = GetFuturePos().x;

    if (nextX >= m_rightX)
    {
        SetMove(m_rightX - m_pos.x, 0.0f);
        m_direction = -1;
    }
    else if (nextX <= m_leftX)
    {
        SetMove(m_leftX - m_pos.x, 0.0f);
        m_direction = 1;
    }

    ApplyMove();
}

// 移動方向に応じた左右反転と、現在位置への平行移動を描画行列に反映する。
void EnemyBase::CalcMat()
{
    m_draw = m_pos - m_pCamera->GetPos();
    // Turn 180 degrees around Y when moving left (horizontal sprite flip).
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
    m_bAlive = false;
    m_invincibleFrames = 0;
    m_hurtFrames = 0;
    m_knockbackX = 0.0f;
    SetMove(0.0f, 0.0f);
    m_texture.Release();
}
