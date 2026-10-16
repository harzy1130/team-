#pragma once
#include"Constants/Constants.h"
#include<utility>

class Map
{
public:
	Map() {};
	~Map() {};
	
	void Init();
	void Update();
	void Draw(Math::Vector2 a_pos);
	void Create();
	void Load();

	void SetTexture(KdTexture* a_pTex) { m_pTex = a_pTex; }

	bool GetCollisionFlg(int h, int w);
	Math::Vector2 GetPosition(int h, int w);
	//firstÇ™â°ÅAsecondÇ™èc
	std::pair<int, int> GetIndex(Math::Vector2 a_pos);
	std::pair<int, int> GetIdxMaxMin() { return std::make_pair(MAP_WIDTH - 1, MAP_HEIGHT - 1); }

private:

	KdTexture*    m_pTex;
	S_ChipData    m_chip[MAP_HEIGHT][MAP_WIDTH];

	const int stageNum = 1;
};