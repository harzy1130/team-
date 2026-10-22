#pragma once

class C_Camera {
public:
	C_Camera() = default;
	~C_Camera() = default;


	void Update();
	void Init();

	//セッター
	void SetTurget(Math::Vector2 a_targetPos) { m_target = a_targetPos; }
	
	//ゲッター
	Math::Vector2 GetPos() { return m_pos; }
private:
	Math::Vector2 m_pos;
	Math::Vector2 m_target;
	//firstが最大、secondが最少
	std::pair<Math::Vector2, Math::Vector2> m_range;
};