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
    if (GetAsyncKeyState('E') && GetAsyncKeyState('1') & 0x8000) {
        Math::Vector2 pos = m_camera->GetPos();
        SpawnEnemy1(pos.x, pos.y);
    }
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
    for (std::size_t i = 0; i < m_enemies1.size(); ++i)
    {
        m_enemies1[i]->SetDamageTestEnabled(i == m_debugEnemy);
        m_enemies1[i]->Update(player, true);
    }
    //æ‚ÉŠÇ—”z—ñ‚Ì—v‘f‚ðíœ
    m_pAllEnemies.erase(std::remove_if(m_pAllEnemies.begin(), m_pAllEnemies.end(),
        [](const EnemyBase* pEB) {
            if (pEB->GetAlive())return false;
            return true;
        }),
        m_pAllEnemies.end());

	m_enemies1.erase(std::remove_if(m_enemies1.begin(), m_enemies1.end(),
        [](const std::unique_ptr<Enemy1>& enemy) {
            if (enemy->GetAlive())return false;
			enemy->Release();
			return true;
        }),
		m_enemies1.end());
    if (m_debugEnemy >= m_enemies1.size())
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
        if (!SpawnEnemy1(spawn))continue;
    }
    return true;
}

bool EnemyManager::SpawnEnemy1(Enemy1::SpawnSettings a_spawn)
{
    auto enemy = std::make_unique<Enemy1>();
    if (!enemy->Init(m_hit, m_camera, m_map, a_spawn) || !enemy->PlaceOnMap(m_map))
    {
        OutputDebugStringA("Enemy spawn failed: texture or ground unavailable.\n");
        return false;
    }
    EnemyBase* enemyBase = enemy.get();
    m_enemies1.push_back(std::move(enemy));
    m_pAllEnemies.push_back(enemyBase);
    return true;
}

bool EnemyManager::SpawnEnemy1(float a_x, float a_y)
{
    Enemy1::SpawnSettings spawn;
    spawn.x = a_x;
    spawn.y = a_y;
    spawn.leftX = a_x + PTEX_SIZE * 2;
    spawn.rightX = a_x - PTEX_SIZE * 2;
    auto enemy = std::make_unique<Enemy1>();
    if (!enemy->Init(m_hit, m_camera, m_map, spawn) || !enemy->PlaceOnMap(m_map))
    {
        OutputDebugStringA("Enemy spawn failed: texture or ground unavailable.\n");
        return false;
    }
    EnemyBase* enemyBase = enemy.get();
    m_enemies1.push_back(std::move(enemy));
    m_pAllEnemies.push_back(enemyBase);
    return true;
}

void EnemyManager::DrawVision()
{
    for (const auto& enemy : m_enemies1) enemy->DrawVision();
}

void EnemyManager::Draw()
{
    for (const auto& allenemy : m_pAllEnemies)allenemy->Draw();
}

std::size_t EnemyManager::GetAliveCount() const
{
    std::size_t count = 0;
    for (const auto& enemy : m_enemies1) if (enemy->GetAlive()) ++count;
    return count;
}

EnemyBase* EnemyManager::GetEnemy(std::size_t index)
{
    return index < m_pAllEnemies.size() ? m_pAllEnemies[index] : nullptr;
}

const EnemyBase* EnemyManager::GetEnemy(std::size_t index) const
{
    return index < m_pAllEnemies.size() ? m_pAllEnemies[index] : nullptr;
}

Enemy1* EnemyManager::GetEnemy1(std::size_t index)
{
    return index < m_enemies1.size() ? m_enemies1[index].get() : nullptr;
}

const Enemy1* EnemyManager::GetEnemy1(std::size_t index) const
{
    return index < m_enemies1.size() ? m_enemies1[index].get() : nullptr;
}

void EnemyManager::SetDebugEnemy(std::size_t index)
{
    m_debugEnemy = index < m_enemies1.size() ? index : 0;
}

void EnemyManager::ClearEnemies()
{
    m_pAllEnemies.clear();
    for (const auto& enemy : m_enemies1) enemy->Release();
    m_enemies1.clear();
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
