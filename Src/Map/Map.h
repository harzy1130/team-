#pragma once
#include"Constants/Constants.h"
#include"Camera/Camera.h"
#include<utility>

class C_Camera;
class C_Map
{
public:
	C_Map() {};
	~C_Map() {};
	
	void Init(C_Camera* a_pCamera);
	void Update();
	void Draw();
	void Create();
	void Load();

	void SetTexture(KdTexture* a_pTex) { m_pTex = a_pTex; }

	bool GetCollisionFlg(int h, int w);
	Math::Vector2 GetPosition(int h, int w);
	//firstが横、secondが縦
	std::pair<int, int> GetIndex(Math::Vector2 a_pos);
	std::pair<int, int> GetIdxMaxMin() { return std::make_pair(MAP_WIDTH - 1, MAP_HEIGHT - 1); }

private:

	KdTexture*    m_pTex;
	S_ChipData    m_chip[MAP_HEIGHT][MAP_WIDTH];

	//参照する別クラスのポインタ
	C_Camera* m_pCamera;

	//仮のステージ選択
	const int stageNum = StageNum::stage1;
};