#include"Map.h"
#include"StringUtility.h"
#include"IntParseResult.h"

void C_Map::Init(C_Camera* a_pCamera)
{
	m_pCamera = a_pCamera;
	m_stageNum = StageNum::stageSelect;
	Load();
	Create();
}

void C_Map::Update()
{
	for (int h = 0; h < GetHeight(); h++)
	{
		for (int w = 0; w < GetWidth(); w++)
		{
			m_drawList[h][w] = m_posList[h][w] - m_pCamera->GetPos();
			m_matList[h][w] = Math::Matrix::CreateTranslation(
				m_drawList[h][w].x, m_drawList[h][w].y, 0);
		}
	}
}

void C_Map::Draw()
{
	for (int h = 0; h < GetHeight(); h++)
	{
		for (int w = 0; w < GetWidth(); w++)
		{
			if (m_drawList[h][w].x > RIGHT_EDGE + CHIP_RADIUS || m_drawList[h][w].x < LEFT_EDGE - CHIP_RADIUS)continue;
			if (m_drawList[h][w].y > TOP_EDGE + CHIP_RADIUS || m_drawList[h][w].y < BOTTOM_EDGE - CHIP_RADIUS)continue;

			SHADER.m_spriteShader.SetMatrix(m_matList[h][w]);//画像の行列をセット
			switch (m_dataList[h][w])
			{
			case MapChip::dirt:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::topWall:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE * 2, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::stone:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE * 3, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::topLeft:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE * 4, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::topRight:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE * 5, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::bottomLeft:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE * 6, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::bottomRight:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE * 7, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::leftWall:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE * 8, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::rightWall:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE * 9, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::topLeftCorner:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE * 10, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::topRightCorner:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE * 11, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::bottomLeftCorner:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE * 12, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::bottomRightCorner:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE * 13, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::bottomWall:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(CHIP_SIZE * 14, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
				break;
			case MapChip::gate1:
			case MapChip::gate2:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(0, 0, CHIP_SIZE, CHIP_SIZE), 1.0f);
			default:
				break;
			}
		}
	}
}

void C_Map::Load()
{

	std::ifstream ifs("Src/Map/MapData/Stage" + std::to_string(m_stageNum) + ".csv");
	if (!ifs.is_open())return;

	m_dataList.clear();
	m_collusionFlgList.clear();

	std::string lineString;

	while (std::getline(ifs, lineString))
	{
		std::istringstream iss(lineString);
		std::string tempString;
		std::vector<int> tempList;
		std::vector<bool> tempCollFlgList;

		while (std::getline(iss, tempString, ','))
		{
			if (tempString == "")tempString = '0';
			int tempData = std::stoi(tempString);
			tempList.push_back(tempData);

			bool tempCollFlg;
			if (tempData != MapChip::none &&
				tempData != MapChip::topRightCorner &&
				tempData != MapChip::topRightCorner &&
				tempData != MapChip::bottomLeftCorner &&
				tempData != MapChip::bottomRightCorner &&
				tempData != MapChip::gate1 &&
				tempData != MapChip::gate2)tempCollFlg = true;
			else tempCollFlg = false;
			tempCollFlgList.push_back(tempCollFlg);
		}
		tempList.push_back(0);
		tempCollFlgList.push_back(false);
		m_dataList.push_back(tempList);
		m_collusionFlgList.push_back(tempCollFlgList);
	}

	// 行ごとの列数を、マップ内で最も長い行に揃える。
	size_t maxWidth = 0;
	for (const auto& row : m_dataList)
	{
		if (row.size() > maxWidth) maxWidth = row.size();
	}
	for (size_t row = 0; row < m_dataList.size(); ++row)
	{
		m_dataList[row].resize(maxWidth, 0);
		m_collusionFlgList[row].resize(maxWidth, false);
	}
	ifs.close();
}
void C_Map::Create()
{
	m_posList.clear();
	m_matList.clear();
	m_drawList.clear();

	for (int h = 0; h < GetHeight(); h++)
	{
		std::vector<Math::Vector2> tempPosList;
		std::vector<Math::Matrix> tempMatList;
		for (int w = 0; w < GetWidth(); w++)
		{
			Math::Vector2 tempPos;
			tempPos.x = LEFT_EDGE + (CHIP_SIZE * w);
			tempPos.y = TOP_EDGE - (CHIP_SIZE * h);

			tempPosList.push_back(tempPos);
			tempMatList.push_back(Math::Matrix::Identity);
		}
		m_posList.push_back(tempPosList);
		m_drawList.push_back(tempPosList);
		m_matList.push_back(tempMatList);
	}
}

bool C_Map::GetCollisionFlg(int h, int w)
{
	if (h >= GetHeight() || w >= GetWidth())return true;
	return m_collusionFlgList[h][w];
}

Math::Vector2 C_Map::GetPosition(int h, int w)
{
	if (h >= GetHeight() || w >= GetWidth())return Math::Vector2{ 0,0 };
	return { m_posList[h][w].x ,m_posList[h][w].y };
}

std::pair<int, int> C_Map::GetIndex(Math::Vector2 a_pos)
{
	std::pair<int, int> index;
	index.first = (a_pos.x - LEFT_EDGE) / CHIP_SIZE;
	index.second = (TOP_EDGE - a_pos.y) / CHIP_SIZE;

	return index;
}

std::pair<int, int> C_Map::GetIndex(float x, float y)
{
	std::pair<int, int> index;
	index.first = (x - LEFT_EDGE) / CHIP_SIZE;
	index.second = (TOP_EDGE - y) / CHIP_SIZE;

	return index;
}