#include "Pch.h"
#include "EnemyManager.h"
#include "Map/Map.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>

bool EnemyManager::Init(C_Hit* hit, C_Camera* camera, C_Map* map, const char* spawnFile)
{
    Release();
    m_hit = hit;
    m_camera = camera;
    m_map = map;
    m_spawnFile = spawnFile != nullptr ? spawnFile : "";
    return m_hit != nullptr && m_camera != nullptr && m_map != nullptr && !m_spawnFile.empty();
}

void EnemyManager::Update(C_PlayerBase* player, bool active)
{
    if (!active || m_map == nullptr || m_hit == nullptr || m_camera == nullptr)
    {
        if (m_wasActive) ClearEnemies();
        m_wasActive = false;
        m_stage = -1;
        return;
    }
    if (!m_wasActive || m_stage != m_map->GetStageNum())
    {
        LoadStage(m_map->GetStageNum());
        m_stage = m_map->GetStageNum();
        m_wasActive = true;
    }
    for (std::size_t i = 0; i < m_enemies.size(); ++i)
    {
        m_enemies[i]->SetDamageTestEnabled(i == m_debugEnemy);
        m_enemies[i]->Update(player, true);
    }
	m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
        [](const std::unique_ptr<Enemy1>& enemy) {
            if (enemy->GetAlive())return false;
			enemy->Release();
			return true;
        }),
		m_enemies.end());
    if (m_debugEnemy >= m_enemies.size())
    {
        m_debugEnemy = 0;
    }
}

bool EnemyManager::LoadStage(int stage)
{
    ClearEnemies();
    std::ifstream file(m_spawnFile);
    if (!file)
    {
        OutputDebugStringA("Enemy spawn data could not be opened.\n");
        return false;
    }
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#' || line.find("stage,") == 0) continue;
        std::replace(line.begin(), line.end(), ',', ' ');
        std::istringstream row(line);
        int rowStage = -1, type = 0;
        Enemy1::SpawnSettings spawn;
        std::string extra;
        if (!(row >> rowStage >> type >> spawn.x >> spawn.y >> spawn.leftX >> spawn.rightX)
            || (row >> extra))
        {
            OutputDebugStringA("Invalid enemy spawn row was skipped.\n");
            continue;
        }
        if (rowStage != stage) continue;
        if (type != 1 || !std::isfinite(spawn.x) || !std::isfinite(spawn.y)
            || !std::isfinite(spawn.leftX) || !std::isfinite(spawn.rightX)
            || spawn.leftX > spawn.rightX || spawn.x < spawn.leftX || spawn.x > spawn.rightX)
        {
            OutputDebugStringA("Unsupported enemy type or invalid spawn range was skipped.\n");
            continue;
        }
        auto enemy = std::make_unique<Enemy1>();
        if (!enemy->Init(m_hit, m_camera, m_map, spawn) || !enemy->PlaceOnMap(m_map))
        {
            OutputDebugStringA("Enemy spawn failed: texture or ground unavailable.\n");
            continue;
        }
        m_enemies.push_back(std::move(enemy));
    }
    return true;
}

void EnemyManager::DrawVision()
{
    for (const auto& enemy : m_enemies) enemy->DrawVision();
}

void EnemyManager::Draw()
{
    for (const auto& enemy : m_enemies) enemy->Draw();
}

std::size_t EnemyManager::GetAliveCount() const
{
    std::size_t count = 0;
    for (const auto& enemy : m_enemies) if (enemy->GetAlive()) ++count;
    return count;
}

Enemy1* EnemyManager::GetEnemy(std::size_t index)
{
    return index < m_enemies.size() ? m_enemies[index].get() : nullptr;
}

const Enemy1* EnemyManager::GetEnemy(std::size_t index) const
{
    return index < m_enemies.size() ? m_enemies[index].get() : nullptr;
}

void EnemyManager::SetDebugEnemy(std::size_t index)
{
    m_debugEnemy = index < m_enemies.size() ? index : 0;
}

void EnemyManager::ClearEnemies()
{
    for (const auto& enemy : m_enemies) enemy->Release();
    m_enemies.clear();
    m_debugEnemy = 0;
}

void EnemyManager::Release()
{
    ClearEnemies();
    m_hit = nullptr;
    m_camera = nullptr;
    m_map = nullptr;
    m_stage = -1;
    m_wasActive = false;
    m_spawnFile.clear();
}
