#pragma once
#include"Map/Map.h"

class C_Map;
class C_Camera {
public:
	C_Camera() = default;
	~C_Camera() = default;


	void Update();
	void Init(C_Map* a_pMap);

	void Shake(float a_power, int a_time) { m_shakePower = a_power; m_shakeTime = a_time; }
	void Zoom(float a_zoomSize) {m_zoomSize = a_zoomSize;}

	//セッター
	void SelectTarget(Math::Vector2* a_targetPos) { m_pTarget = a_targetPos; }
	//ゲッター
	Math::Vector2 GetPos() { return m_pos; }
private:
	void ShakeUpdate();
	void ZoomUpdate();
	C_Map* m_pMap = nullptr;

	Math::Vector2 m_pos;
	Math::Vector2 m_target;
	Math::Vector2* m_pTarget = nullptr;
	//firstが最大、secondが最少
	std::pair<Math::Vector2, Math::Vector2> m_range;
	float m_shakePower = 0;
	int m_shakeTime = 0;
	float m_zoomSize = 1.0f;
};