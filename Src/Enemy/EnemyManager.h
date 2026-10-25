#pragma once
#include "Enemy1.h"
#include <memory>
#include <string>
#include <vector>

// ステージごとの配置データから敵を生成し、更新・描画・解放をまとめて行う。
class EnemyManager
{
public:
    bool Init(C_Hit* hit, C_Camera* camera, C_Map* map,
        const char* spawnFile = "Src/Enemy/EnemySpawns.csv");
    void Update(C_PlayerBase* player, bool active);
    void DrawVision();
    void Draw();
    void Release();

    std::size_t GetCount() const { return m_pAllEnemies.size(); }
    std::size_t GetAliveCount() const;
    Enemy1* GetEnemy(std::size_t index);
    const Enemy1* GetEnemy(std::size_t index) const;
    void SetDebugEnemy(std::size_t index);
    bool SpawnEnemy1(Enemy1::SpawnSettings a_spawn);
    bool SpawnEnemy1(float a_x,float a_y);
    

private:
    bool LoadStage(int stage);
    void ClearEnemies();
    // テクスチャを持つ敵本体はコピーせず、所有ポインターで保持する。
    std::vector<std::unique_ptr<Enemy1>> m_enemies1;
    C_Hit* m_hit = nullptr;
    C_Camera* m_camera = nullptr;
    C_Map* m_map = nullptr;
    std::string m_spawnFile;
    int m_stage = -1;
    bool m_wasActive = false;
    std::size_t m_debugEnemy = 0;

    // m_enemies が所有し、ここでは EnemyBase として参照する非所有ポインター一覧。
    std::vector<EnemyBase*> m_pAllEnemies;
};
