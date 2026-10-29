#include "Pch.h"
#include "CharacterGUI.h"
#include "Player/PlayerBase.h"
#include "Enemy/EnemyManager.h"
#include "GameStateManager/GameStateManager.h"
#include "Map/Map.h"

// 各 getter から最新の値を取得し、プレイヤーと敵の情報を並べて表示する。
void CharacterGUI::Draw(C_PlayerBase& player, EnemyManager& enemies, C_GameStateManager& game, C_Map& map)
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
        ImGui::TextUnformatted(u8"敵の情報");
        ImGui::Separator();
        const int count = static_cast<int>(enemies.GetCount());
        ImGui::Text(u8"配置数: %d / 生存数: %d", count, static_cast<int>(enemies.GetAliveCount()));
        m_selectedEnemy = count > 0 ? std::max(0, std::min(m_selectedEnemy, count - 1)) : 0;
        if (count > 1) ImGui::SliderInt(u8"表示する敵", &m_selectedEnemy, 0, count - 1);
        enemies.SetDebugEnemy(static_cast<std::size_t>(m_selectedEnemy));
        const Enemy1* enemy = enemies.GetEnemy1(static_cast<std::size_t>(m_selectedEnemy));
        if (enemy != nullptr)
        {
            ImGui::Text("HP: %d", enemy->GetHp());
            ImGui::Text(u8"攻撃力 (ATK): %d", enemy->GetAtk());
            ImGui::Text(u8"巡回速度: %.1f px/frame", enemy->GetMoveSpeed());
            const auto enemyPos = enemy->GetPos();
            ImGui::Text(u8"座標 X: %.1f", enemyPos.x);
            ImGui::Text(u8"座標 Y: %.1f", enemyPos.y);
            ImGui::Text(u8"足元 Y: %.1f", enemy->GetHitBottom());
            ImGui::Text(u8"状態: %s", enemy->GetAlive() ? u8"生存" : u8"死亡");
            ImGui::Text(u8"被ダメージ中: %s", enemy->IsHurt() ? u8"はい" : u8"いいえ");
            ImGui::Text(u8"無敵時間中: %s", enemy->IsInvincible() ? u8"はい" : u8"いいえ");
            ImGui::Text(u8"開幕スキル使用済み: %s", enemy->HasUsedOpening() ? u8"はい" : u8"いいえ");
            ImGui::Spacing();
            ImGui::TextUnformatted(u8"H: 選択中の敵に25ダメージ（テスト）");
        }
        else ImGui::TextUnformatted(u8"このステージには敵が配置されていません。");
        ImGui::TextUnformatted(u8"1 + E で新たに敵を配置。");

        ImGui::Columns(1);

        ImGui::Columns(2, "", true);
        ImGui::Separator();
        ImGui::TextUnformatted(u8"ゲームの状態");
        if (game.GetGameState() == GameState::Title)ImGui::Text(u8"タイトル");
        if (game.GetGameState() == GameState::Select)ImGui::Text(u8"セレクト");
        if (game.GetGameState() == GameState::Game)ImGui::Text(u8"ゲーム");

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text(u8"T: タイトル\nR: セレクトステージ\n1: テストステージ ");

        ImGui::NextColumn();
        ImGui::TextUnformatted(u8"現在のステージ");
        if (map.GetStageNum() == StageNum::stage1)ImGui::Text(u8"ステージ１");
        if (map.GetStageNum() == StageNum::stage2)ImGui::Text(u8"ステージ２");
        if (map.GetStageNum() == StageNum::stageSelect)ImGui::Text(u8"セレクトステージ");
        if (map.GetStageNum() == StageNum::Test)ImGui::Text(u8"テストステージ");
    }
    // Begin が false の場合も、対応する End は必ず呼び出す。
    ImGui::End();
}