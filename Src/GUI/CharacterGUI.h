#pragma once

class C_PlayerBase;
class EnemyManager;
class C_GameStateManager;
class C_Map;

class CharacterGUI
{
private:
    int m_selectedEnemy = 0;
public:
	// プレイヤーと敵の現在の情報を、読み取り専用の GUI に表示する。
	void Draw(C_PlayerBase& player, EnemyManager& enemies, C_GameStateManager& game, C_Map& map);
};
