#include"Map.h"
#include"StringUtility.h"
#include"IntParseResult.h"

void Map::Update()
{
	for (int h = 0; h < MAP_HEIGHT; h++)
	{
		for (int w = 0; w < MAP_WIDTH; w++)
		{
			m_chip[h][w].mat = Math::Matrix::CreateTranslation(m_chip[h][w].pos.x, m_chip[h][w].pos.y, 0);
		}
	}

}

void Map::Draw(Math::Vector2 a_pos)
{
	for (int h = 0; h < MAP_HEIGHT; h++)
	{
		for (int w = 0; w < MAP_WIDTH; w++)
		{
			m_chip[h][w].mat= Math::Matrix::CreateTranslation(m_chip[h][w].pos.x - a_pos.x, m_chip[h][w].pos.y - a_pos.y,0);
			SHADER.m_spriteShader.SetMatrix(m_chip[h][w].mat);//画像の行列をセット

			switch (m_chip[h][w].data)
			{
			//case 0:
			//case 3:
			//	SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(0, 0, PTEX_SIZE, PTEX_SIZE), 1.0f);
				//break;
				case MapChip::dirt:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(PTEX_SIZE, 0, PTEX_SIZE, PTEX_SIZE), 1.0f);
				break;
				case MapChip::glass:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(PTEX_SIZE * 2, 0, PTEX_SIZE, PTEX_SIZE), 1.0f);
				break;
			default:
				break;
			}
		}
	}
}

void Map::Create()
{
	for (int h = 0; h < MAP_HEIGHT; h++)
	{
		for (int w = 0; w < MAP_WIDTH; w++)
		{
			m_chip[h][w].pos.x = LEFT_EDGE + w * PTEX_SIZE;
			m_chip[h][w].pos.y = TOP_EDGE  - h * PTEX_SIZE;
			m_chip[h][w].mat   = Math::Matrix::CreateTranslation(m_chip[h][w].pos.x, m_chip[h][w].pos.y, 0);
		}
	}
}

void Map::Load()
{
	for (int h = 0; h < MAP_HEIGHT; h++)
	{
		for (int w = 0; w < MAP_WIDTH; w++)
		{
			m_chip[h][w].data = 0;
		}
	}

	std::string value = "";
	std::vector<std::string> list;
	FILE* mapfp;
	std::string filename = "Src/Map/MapData/Stage" + std::to_string(stageNum) + ".csv";

	if ((mapfp = fopen(filename.c_str(), "r")) != nullptr)
	{
		char ch;
		while ((ch = fgetc(mapfp)) != EOF)
		{
			if (ch == ',')
			{
				list.push_back(value);
				value.clear();
			}
			else if (ch == '\n')
			{
				list.push_back(value);
				list.push_back(NEXT_ROW);
				value.clear();
			}
			else
			{
				value += ch;
			}
		}
	}
	else
	{
		return;
	}
	fclose(mapfp);

	int h = 0;
	int w = 0;
	for (int i = 0; i < list.size(); i++)
	{
		IntParseResult result = StringUtility::TryIntParse(list[i]);

		if (result.IsSuccess)
		{
			m_chip[h][w].data = result.Number;
			if (m_chip[h][w].data == 0)m_chip[h][w].collisionFlg = false;
			else m_chip[h][w].collisionFlg = true;
			w++;
		}
		else
		{
			h++;
			w = 0;
		}
	}
}

bool Map::GetCollisionFlg(int h, int w)
{
	if (h >= MAP_HEIGHT || w >= MAP_WIDTH)return true;
	return m_chip[h][w].collisionFlg;
}

std::pair<int, int> Map::GetIndex(Math::Vector2 a_pos)
{
	std::pair<int, int> index;
	index.first  = (a_pos.x - LEFT_EDGE) / PTEX_SIZE;
	index.second = (TOP_EDGE - a_pos.y)  / PTEX_SIZE;

	return index;
}
