#pragma once
#include"MapConstants.h"

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
	void Draw();
	void Create();
	void Load();

	void SetTexture(KdTexture* a_pTex) { m_pTex = a_pTex; }

	int GetStageNum() { return stageNum; }
private:

	KdTexture*    m_pTex;
	int		      m_data[MapHeight][MapWidth];
	Math::Vector2 m_pos[MapHeight][MapWidth];
	Math::Matrix  m_mat[MapHeight][MapWidth];


	//std::vector<std::vector<ChipData>>m_chip;
	//int m_height;
	//int m_width;

	const int stageNum = 1;
};