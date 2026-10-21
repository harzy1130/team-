#include "Pch.h"
#include "CharacterGUI.h"
#include "Player/PlayerBase.h"
#include "Enemy/EnemyBase.h"
#include "GameStateManager/GameStateManager.h"
#include "Map/Map.h"

// 各 getter から最新の値を取得し、プレイヤーと敵の情報を並べて表示する。
void CharacterGUI::Draw(C_PlayerBase& player, const EnemyBase& enemy, C_GameStateManager& game, C_Map& map)
{
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(550, 420), ImGuiSetCond_Once);
    if (ImGui::Begin(u8"キャラクター情報"))
    {
        ImGui::Columns(2, "CharacterStats", true);

        ImGui::TextUnformatted(u8"プレイヤー");
        ImGui::Separator();
        ImGui::Text(u8"HP: %.0f / %.0f", player.GetHP(), player.GetMAXHP());
        ImGui::Text(u8"攻撃力 (ATK): %.0f", player.GetATK());
        ImGui::Text(u8"防御力 (DEF): %.0f", player.GetDEF());
        ImGui::Text(u8"攻撃速度 (AS): %.0f", player.GetAS());
        // MS は基本速度ではなく、既存の Getter が返す補正値。
        ImGui::Text(u8"移動速度補正 (MS): %.1f %%", player.GetMS());
        ImGui::Text(u8"ライフスティール: %.1f", player.GetLifStl());
        ImGui::Text(u8"クリティカル率: %.1f %%", player.GetCrtRate());
        ImGui::Text(u8"クリティカル倍率: %.1f %%", player.GetCrtDmg());
        ImGui::Spacing();
        const auto playerPos = player.GetPos();
        const auto playerMove = player.GetMove();
        ImGui::Text(u8"座標 X: %.1f", playerPos.x);
        ImGui::Text(u8"座標 Y: %.1f", playerPos.y);
        ImGui::Text(u8"移動量 X: %.1f", playerMove.x);
        ImGui::Text(u8"移動量 Y: %.1f", playerMove.y);
        ImGui::Text(u8"状態: %s", player.GetAFlg() ? u8"生存" : u8"無効");
        ImGui::Text(u8"ジャンプ中: %s", player.GetJumpFlg() ? u8"はい" : u8"いいえ");

        ImGui::NextColumn();
        ImGui::TextUnformatted(u8"敵 (Enemy1)");
        ImGui::Separator();
        ImGui::Text("HP: %d", enemy.GetHp());
        ImGui::Text(u8"攻撃力 (ATK): %d", enemy.GetAtk());
        ImGui::Text(u8"巡回速度: %.1f px/frame", enemy.GetMoveSpeed());
        ImGui::Spacing();
        const auto enemyPos = enemy.GetPos();
        ImGui::Text(u8"座標 X: %.1f", enemyPos.x);
        ImGui::Text(u8"座標 Y: %.1f", enemyPos.y);
        ImGui::Text(u8"足元 Y: %.1f", enemy.GetHitBottom());
        ImGui::Text(u8"状態: %s", enemy.GetAlive() ? u8"生存" : u8"死亡");
        ImGui::Text(u8"被ダメージ中: %s", enemy.IsHurt() ? u8"はい" : u8"いいえ");
        ImGui::Text(u8"無敵時間中: %s", enemy.IsInvincible() ? u8"はい" : u8"いいえ");
        ImGui::Spacing();
        ImGui::TextUnformatted(u8"H: 敵に25ダメージ（テスト）");

        ImGui::Columns(1);

        ImGui::NextColumn();
        ImGui::Separator();
        ImGui::TextUnformatted(u8"GameState");
        ImGui::Text("GameState: %d", game.GetGameState());
        ImGui::Text("StageNum: %d", map.GetStageNum());
        if (ImGui::Button(u8"テストステージに移動"))
        {
            map.SetStageNum(StageNum::Test);
            map.Load();
            map.Create();
            game.SetGameState(GameState::Game);
        }
    }
    // Begin が false の場合も、対応する End は必ず呼び出す。
    ImGui::End();
}