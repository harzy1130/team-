#pragma once

class C_PlayerBase;
class EnemyBase;
class C_GameStateManager;
class C_Map;

class CharacterGUI
{
public:
	// プレイヤーと敵の現在の情報を、読み取り専用の GUI に表示する。
	void Draw(C_PlayerBase& player, const EnemyBase& enemy, C_GameStateManager& game, C_Map& map);
};
