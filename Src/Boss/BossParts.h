#pragma once
#include"BossBase.h"

class C_BossParts : public C_BossBase
{
public:
	void Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_PlayerBase* a_pPlayer) override;
	void Update() override;
	void Move() override {};
	void Release() override {};

	void SetParent(C_BossBase* a_pParent) { m_pParent = a_pParent; }
	void SetPos(Math::Vector2 a_pos) { m_pos = a_pos; }
	void SetTexturePath(const std::string& a_path) { m_texture.Load(a_path); }
	void SetOffsetPos(Math::Vector2 a_offset) { m_offsetPos = a_offset; }
	void SetAngle(float a_angle) { m_angle = a_angle; }
	void SetOffsetAngle(float a_angle) { m_offsetAngle = a_angle; }

	Math::Vector2 GetPos() { return m_pos; }
	Math::Vector2 GetOffsetPos() { return m_offsetPos; }
	Math::Vector2 GetStartPos() { return m_startPos; }
	float GetOffsetAngle() { return m_offsetAngle; }
private:
	C_BossBase* m_pParent = nullptr;
	Math::Vector2 m_offsetPos = { 0,0 };
	Math::Vector2 m_startPos = { 0,0 };
	float m_parentAngle = 0;
	float m_offsetAngle = 0;
};