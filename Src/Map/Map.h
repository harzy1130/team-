#pragma once
#include"MapConstants.h"
#include<utility>

struct ChipData
{
	Math::Matrix mat;
	Math::Vector2 pos;
	Math::Vector2 draw;
	int data;
};

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

	Math::Vector2 GetPosition(int h, int w);
	std::pair<int, int> GetIndex(Math::Vector2 a_pos);
	std::pair<int, int> GetIdxMaxMin() { return std::make_pair(MapWidth - 1, MapHeight - 1); }
private:

	KdTexture*    m_pTex;
	int		      m_data[MapHeight][MapWidth];
	Math::Vector2 m_pos[MapHeight][MapWidth];
	Math::Matrix  m_mat[MapHeight][MapWidth];

	const int stageNum = 1;
};