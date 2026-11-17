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
    if (m_hit == nullptr || m_camera == nullptr || m_map == nullptr || m_spawnFile.empty()) return false;
    // 初回のステージ更新を止めないよう、タイトル表示前に画像を準備する。
    if (!Enemy1::PreloadSprites() || !Enemy2::PreloadSprites())
    {
        OutputDebugStringA("Enemy sprite preload failed.\n");
        return false;
    }
    if (!m_questionIcon.Load("Src/Enemy/Texture/EnemyState_Question.png")
        || !m_alertIcon.Load("Src/Enemy/Texture/EnemyState_Alert.png")
        || !m_attackIcon.Load("Src/Enemy/Texture/EnemyState_Attack.png"))
    {
        OutputDebugStringA("Enemy state icon load failed.\n");
        Release();
        return false;
    }
    return true;
}

void EnemyManager::Update(C_PlayerBase* player, bool active)
{
    // 両キーの押下状態を確認し、組み合わせが成立した瞬間だけ召喚する。
    const bool spawnDown = (GetAsyncKeyState('E') & 0x8000) != 0
        && (GetAsyncKeyState('1') & 0x8000) != 0;
    const bool spawnPressed = spawnDown && !m_debugSpawnWasDown;
    m_debugSpawnWasDown = spawnDown;
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
    // ステージ読み込みと参照先の確認を終えてから生成する。
    if (spawnPressed)
    {
        const auto pos = m_camera->GetPos();
        SpawnEnemy1(pos.x, pos.y);
    }
    const EnemyBase* debugEnemy = GetEnemy(m_debugEnemy);
    for (std::size_t i = 0; i < m_enemies1.size(); ++i)
    {
        m_enemies1[i]->SetDamageTestEnabled(m_enemies1[i].get() == debugEnemy);
        m_enemies1[i]->Update(player, true);
    }
    for (const auto& enemy : m_enemies2)
    {
        enemy->SetDamageTestEnabled(enemy.get() == debugEnemy);
        enemy->Update(player, true);
    }
    //先に管理配列の要素を削除
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
    m_enemies2.erase(std::remove_if(m_enemies2.begin(), m_enemies2.end(),
        [](const std::unique_ptr<Enemy2>& enemy) {
            if (enemy->GetAlive()) return false;
            enemy->Release(); return true;
        }), m_enemies2.end());
    if (m_debugEnemy >= m_pAllEnemies.size())
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
        if ((type != 1 && type != 2) || !std::isfinite(spawn.x) || !std::isfinite(spawn.y)
            || !std::isfinite(spawn.leftX) || !std::isfinite(spawn.rightX)
            || spawn.leftX > spawn.rightX || spawn.x < spawn.leftX || spawn.x > spawn.rightX)
        {
            OutputDebugStringA("Unsupported enemy type or invalid spawn range was skipped.\n");
            continue;
        }
        if (type == 1) SpawnEnemy1(spawn);
        else SpawnEnemy2({spawn.x, spawn.y, spawn.leftX, spawn.rightX});
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
    if (!m_showVision) return;
    for (auto* enemy : m_pAllEnemies) enemy->DrawVision();
}

void EnemyManager::Draw()
{
    for (const auto& allenemy : m_pAllEnemies)allenemy->Draw();
    DrawStatusIndicators();
}

void EnemyManager::DrawStatusIndicators()
{
    const auto camera = m_camera != nullptr ? m_camera->GetPos() : Math::Vector2(0.0f, 0.0f);
    const Math::Color color(1.0f, 1.0f, 1.0f, 1.0f);
    for (const auto* enemy : m_pAllEnemies)
    {
        KdTexture* icon = nullptr;
        int size = 32;
        switch (enemy->GetAwarenessState())
        {
        case EnemyBase::AwarenessState::Suspicious: icon = &m_questionIcon; break;
        case EnemyBase::AwarenessState::Alert: icon = &m_alertIcon; break;
        case EnemyBase::AwarenessState::Attack: icon = &m_attackIcon; size = 40; break;
        default: continue;
        }
        const auto& info = icon->GetInfo();
        if (info.Width == 0 || info.Height == 0) continue;
        // 敵の向きで反転せず、ワールド座標の右上からカメラ位置を一度だけ引く。
        SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(
            enemy->GetHitRight() + 8.0f - camera.x,
            enemy->GetHitTop() + 24.0f - camera.y, 0.0f));
        const Math::Rectangle source(0, 0, static_cast<int>(info.Width), static_cast<int>(info.Height));
        SHADER.m_spriteShader.DrawTex(icon, 0, 0, size, size, &source, &color, {0.5f, 0.5f});
    }
    SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

std::size_t EnemyManager::GetAliveCount() const
{
    std::size_t count = 0;
    for (const auto* enemy : m_pAllEnemies) if (enemy->GetAlive()) ++count;
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
    m_debugEnemy = index < m_pAllEnemies.size() ? index : 0;
}

void EnemyManager::ClearEnemies()
{
    m_pAllEnemies.clear();
    for (const auto& enemy : m_enemies1) enemy->Release();
    m_enemies1.clear();
    for (const auto& enemy : m_enemies2) enemy->Release();
    m_enemies2.clear();
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
    m_debugSpawnWasDown = false;
    m_showVision = false;
    m_questionIcon.Release();
    m_alertIcon.Release();
    m_attackIcon.Release();
}

Enemy2* EnemyManager::GetEnemy2(std::size_t index)
{
    return index < m_enemies2.size() ? m_enemies2[index].get() : nullptr;
}

const Enemy2* EnemyManager::GetEnemy2(std::size_t index) const
{
    return index < m_enemies2.size() ? m_enemies2[index].get() : nullptr;
}

bool EnemyManager::SpawnEnemy2(const Enemy2::SpawnSettings& spawn)
{
    auto enemy = std::make_unique<Enemy2>();
    if (!enemy->Init(m_hit, m_camera, m_map, spawn) || !enemy->PlaceOnMap(m_map))
    {
        OutputDebugStringA("Enemy2 spawn failed: texture or ground unavailable.\n");
        return false;
    }
    EnemyBase* base = enemy.get();
    m_enemies2.push_back(std::move(enemy));
    m_pAllEnemies.push_back(base);
    return true;
}