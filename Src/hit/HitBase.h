#pragma once
#include"Pch.h"
#include"Camera/Camera.h"
#include"Constants/Constants.h"

class C_Hit;
class C_Camera;
// World coordinates use +Y upwards. m_rect is relative to m_pos.
class HitBase
{
public:
    // 派生クラスのオブジェクトを基底クラス経由でも安全に破棄できるようにする。
    virtual ~HitBase() = default;

    // デバッグ用の矩形表示
    void DrawDebugRect(Math::Color* color);

    // 現在のワールド座標を設定する。
    void SetPos(float x, float y) { m_pos = Math::Vector2(x, y); }
    // 次に適用する X・Y 方向の移動量を設定する。
    void SetMove(float x, float y) { m_move = Math::Vector2(x, y); }
    // 中心からの半幅・半高さを指定して、ローカル座標の当たり判定矩形を設定する。
    void SetRect(long halfWidth, long halfHeight);
    // 不規則な矩形用。-はつけなくてもよい。
    void SetRect(long left, long top, long right, long bottom);
    // 中心からの半径で円判定用の円を設定
    void SetRadius(float a_radius) { m_radius = a_radius; }
    // どこが当たっているかをリセット
    void ResetHitRect();
    // 左が壁に当たっている
    void SetHitLeft(float a_x);
    // 上が天井に当たっている
    void SetHitTop(float a_y);
    // 右が壁に当たっている
    void SetHitRight(float a_x);
    // 下が床に触れている
    void SetHitBottom(float a_y);
    // 現在のワールド座標を返す。
    Math::Vector2 GetPos() const { return m_pos; }
    // まだ適用していない移動量を返す。
    Math::Vector2 GetMove() const { return m_move; }
    // 現在位置に移動量を加えた、移動後の予測座標を返す。
    Math::Vector2 GetFuturePos() const { return m_pos + m_move; }
    // 物体の中心を基準としたローカル座標の当たり判定矩形を返す。
    RECT GetRect() const { return m_rect; }
    // 現在位置での当たり判定矩形を返す。RECT の座標は整数に変換される。
    RECT GetNowRect() const;
    // 移動後の予測位置での当たり判定矩形を返す。RECT の座標は整数に変換される。
    RECT GetFutureRect() const;

    // 現在の当たり判定の上端をワールド座標で返す（Y 軸は上向きが正）。
    float GetHitTop() const { return m_pos.y + m_rect.top; }
    // 現在の当たり判定の下端をワールド座標で返す。
    float GetHitBottom() const { return m_pos.y + m_rect.bottom; }
    // 現在の当たり判定の左端をワールド座標で返す。
    float GetHitLeft() const { return m_pos.x + m_rect.left; }
    // 現在の当たり判定の右端をワールド座標で返す。
    float GetHitRight() const { return m_pos.x + m_rect.right; }

    // Only positive-area overlap counts as a hit; touching edges do not.
    // 現在位置で相手と矩形が重なるかを調べる。辺が接するだけの場合は false を返す。
    bool IsHit(const HitBase& other) const;
    // Predict overlap after both objects apply their pending moves.
    // 双方が移動量を適用した後に、矩形が重なるかどうかを予測する。
    bool IsFutureHit(const HitBase& other) const;
    // Apply once after any collision correction, then clear the pending move.
    // 移動量を現在位置に一度だけ適用し、適用済みの移動量を 0 に戻す。
    void ApplyMove();

    void SetHitClass(C_Hit* a_pHit) { m_pHit = a_pHit; };
    void SetCameraClass(C_Camera* a_pCamera) { m_pCamera = a_pCamera; };
protected:
    RECT m_rect{};
    S_HitRect m_bHitRect;
    Math::Vector2 m_pos{ 0.0f, 0.0f };
    Math::Vector2 m_draw{ 0.0f, 0.0f };
    Math::Vector2 m_scale{ 1.0f, 1.0f };
    Math::Vector2 m_move{ 0.0f, 0.0f };
    float m_radius = 0;
    C_Camera* m_pCamera = nullptr;
    C_Hit* m_pHit = nullptr;
};
