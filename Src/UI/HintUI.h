#pragma once

#include "Pch.h"

class C_Camera;
class C_PlayerBase;

class HintUI
{
public:
    void Init(C_PlayerBase* player, C_Camera* camera);
    void Update(bool inGame, int stage);
    void Draw() const;
    void Release();

private:
    KdTexture m_panel;
    KdTexture m_keyA;
    KdTexture m_keyD;
    KdTexture m_mouse;
    C_PlayerBase* m_player = nullptr;
    C_Camera* m_camera = nullptr;
    bool m_visible = false;
    float m_leftPress = 0.0f;
    float m_rightPress = 0.0f;
    float m_attackPress = 0.0f;
};
