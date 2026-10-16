#include"Map.h"
#include"StringUtility.h"
#include"IntParseResult.h"

void Map::Update()
{
	for (int h = 0; h < MapHeight; h++)
	{
		for (int w = 0; w < MapWidth; w++)
		{
			m_mat[h][w] = Math::Matrix::CreateTranslation(m_pos[h][w].x, m_pos[h][w].y, 0);
		}
	}

}

void Map::Draw()
{
	for (int h = 0; h < MapHeight; h++)
	{
		for (int w = 0; w < MapWidth; w++)
		{
			SHADER.m_spriteShader.SetMatrix(m_mat[h][w]);//画像の行列をセット

			switch (m_data[h][w])
			{
			//case 0:
			//case 3:
			//	SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(0, 0, TexSize, TexSize), 1.0f);
				//break;
				case MapChip::dirt:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(TexSize, 0, TexSize, TexSize), 1.0f);
				break;
				case MapChip::glass:
				SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(TexSize * 2, 0, TexSize, TexSize), 1.0f);
				break;
			default:
				break;
			}
		}
	}
}

void Map::Create()
{
	for (int h = 0; h < MapHeight; h++)
	{
		for (int w = 0; w < MapWidth; w++)
		{
			m_pos[h][w].x = LeftEdge + w * TexSize;
			m_pos[h][w].y = TopEdge - h * TexSize;
			m_mat[h][w]   = Math::Matrix::CreateTranslation(m_pos[h][w].x, m_pos[h][w].y, 0);
		}
	}
}

void Map::Load()
{
	for (int h = 0; h < MapHeight; h++)
	{
		for (int w = 0; w < MapWidth; w++)
		{
			m_data[h][w] = 0;
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
			m_data[h][w] = result.Number;
			w++;
		}
		else
		{
			h++;
			w = 0;
		}
	}
}

Math::Vector2 Map::GetPosition(int h, int w)
{
	Math::Vector2 pos;
	pos.x = LeftEdge + w * TexSize;
	pos.y = TopEdge - h * TexSize;

	return pos;
}
