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
	void Load();
	void Create();

	void SetTexture(KdTexture* a_pTex) { m_pTex = a_pTex; }
	void SetStageNum(int a_stageNum) { m_stageNum = a_stageNum; }

	Math::Vector2 GetPosition(int h, int w);
	bool GetCollisionFlg(int h, int w);
	int GetData(int h, int w) { return m_dataList[h][w]; }
	int GetStageNum() { return m_stageNum; }

	//firstが横、secondが縦
	float GetHeight() { return m_dataList.size(); }
	float GetWidth() { return m_dataList.begin()->size(); }

	std::pair<int, int> GetIndex(Math::Vector2 a_pos);
	std::pair<int, int> GetIndex(float x, float y);
	std::pair<int, int> GetIdxMaxMin() { return std::make_pair(GetWidth() - 1, GetHeight() - 1); }

private:

	KdTexture* m_pTex;

	std::vector<std::vector<Math::Vector2>> m_posList;
	std::vector<std::vector<Math::Vector2>> m_drawList;
	std::vector<std::vector<Math::Matrix>> m_matList;
	std::vector<std::vector<bool>> m_collusionFlgList;
	std::vector<std::vector<int>>  m_dataList;
	int m_stageNum;

	//参照する別クラスのポインタ
	C_Camera* m_pCamera;
};