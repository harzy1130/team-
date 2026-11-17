#include "Pch.h"
#include "HintUI.h"
#include "Camera/Camera.h"
#include "Constants/Constants.h"
#include "Player/PlayerBase.h"
#include <cmath>

namespace
{
    constexpr int CardWidth = 204;
    constexpr int CardHeight = 54;
    constexpr int CardSpacing = 10;
    const float HeadGap = 15.0f + SCREEN_HEIGHT / 4.0f;
    constexpr int TopMargin = 8;
    constexpr int KeyTravel = 5;

    // 生成画像の透明な余白を除いて描画する。
    const Math::Rectangle PanelSource{59, 171, 2054, 371};
    const Math::Rectangle KeyASource{233, 259, 812, 774};
    const Math::Rectangle KeyDSource{211, 229, 832, 824};
    const Math::Rectangle MouseSource{278, 213, 626, 900};
    const Math::Color White{1.0f, 1.0f, 1.0f, 1.0f};
    const Math::Color Label{0.96f, 0.98f, 1.0f, 1.0f};
    const Math::Color MovePressTint{0.62f, 0.87f, 1.0f, 1.0f};
    const Math::Color AttackPressTint{1.0f, 0.79f, 0.56f, 1.0f};

    float AnimatePress(float value, bool pressed)
    {
        constexpr float Step = 0.34f;
        return pressed ? std::min(1.0f, value + Step) : std::max(0.0f, value - Step);
    }

    bool IsOnScreen(int x, int y)
    {
        return x + CardWidth / 2 >= LEFT_EDGE && x - CardWidth / 2 <= RIGHT_EDGE
            && y + CardHeight / 2 >= BOTTOM_EDGE && y - CardHeight / 2 <= TOP_EDGE;
    }

    void DrawButton(const KdTexture& texture, const Math::Rectangle& source,
        int x, int y, int width, int height, float press, const Math::Color& pressedTint)
    {
        const int offset = static_cast<int>(std::lround(KeyTravel * press));
        const Math::Color tint = Math::Color::Lerp(White, pressedTint, press);
        SHADER.m_spriteShader.DrawTex(&texture, x, y - offset, width, height,
            &source, &tint, {0.5f, 0.5f});
    }
}

void HintUI::Init(C_PlayerBase* player, C_Camera* camera)
{
    Release();
    const std::string path = "Src/UI/Texture/HintUI/";
    if (!m_panel.Load(path + "Panel.png") || !m_keyA.Load(path + "KeyA.png")
        || !m_keyD.Load(path + "KeyD.png") || !m_mouse.Load(path + "MouseLeft.png"))
    {
        OutputDebugStringA("HintUI: failed to load textures from Src/UI/Texture/HintUI/\n");
        Release();
        return;
    }

    m_player = player;
    m_camera = camera;
}

void HintUI::Release()
{
    m_panel.Release();
    m_keyA.Release();
    m_keyD.Release();
    m_mouse.Release();
    m_player = nullptr;
    m_camera = nullptr;
    m_visible = false;
    m_leftPress = m_rightPress = m_attackPress = 0.0f;
}

void HintUI::Update(bool inGame, int stage)
{
    m_visible = inGame && stage == StageNum::Test;
    if (!m_visible)
    {
        m_leftPress = m_rightPress = m_attackPress = 0.0f;
        return;
    }

    bool leftPressed = false;
    bool rightPressed = false;
    bool attackPressed = false;
    const HWND window = GetActiveWindow();
    if (window != nullptr && GetForegroundWindow() == window)
    {
        leftPressed = (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000);
        rightPressed = (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000);
        attackPressed = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    }

    m_leftPress = AnimatePress(m_leftPress, leftPressed);
    m_rightPress = AnimatePress(m_rightPress, rightPressed);
    m_attackPress = AnimatePress(m_attackPress, attackPressed);
}

void HintUI::Draw() const
{
    if (!m_visible || m_player == nullptr || m_camera == nullptr || !m_player->GetAFlg()) return;

    // 描画時のプレイヤー座標とカメラ座標を使い、頭上に固定する。
    const Math::Vector2 cameraPosition = m_camera->GetPos();
    const int x = static_cast<int>(std::lround(m_player->GetPos().x - cameraPosition.x));
    const float preferredY = m_player->GetHitTop() - cameraPosition.y + HeadGap + CardHeight / 2;
    const float topLimit = TOP_EDGE - CardHeight / 2 - TopMargin;
    const int y = static_cast<int>(std::lround(std::min(preferredY, topLimit)));
    const int moveX = x - (CardWidth + CardSpacing) / 2;
    const int attackX = x + (CardWidth + CardSpacing) / 2;
    const bool drawMove = IsOnScreen(moveX, y);
    const bool drawAttack = IsOnScreen(attackX, y);
    if (!drawMove && !drawAttack) return;

    auto& sprite = SHADER.m_spriteShader;
    sprite.SetMatrix(Math::Matrix::Identity);
    if (drawMove)
    {
        sprite.DrawTex(&m_panel, moveX, y, CardWidth, CardHeight, &PanelSource, &White, {0.5f, 0.5f});
        DrawButton(m_keyA, KeyASource, moveX - 75, y + 2, 34, 34, m_leftPress, MovePressTint);
        DrawButton(m_keyD, KeyDSource, moveX - 36, y + 2, 34, 34, m_rightPress, MovePressTint);
    }
    if (drawAttack)
    {
        sprite.DrawTex(&m_panel, attackX, y, CardWidth, CardHeight, &PanelSource, &White, {0.5f, 0.5f});
        DrawButton(m_mouse, MouseSource, attackX - 71, y + 2, 28, 38, m_attackPress, AttackPressTint);
    }

    // SpriteFont が設定を切り替えるため、文字描画後にスプライト描画を再開する。
    sprite.End();
    if (drawMove)
        sprite.DrawString(moveX - 14, y + 17, "MOVE", Label);
    if (drawAttack)
        sprite.DrawString(attackX - 48, y + 17, "ATTACK", Label);
    sprite.Begin();
}
