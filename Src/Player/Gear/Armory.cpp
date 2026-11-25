#include "Armory.h"

void C_Armory::Init()
{
	std::string path= "Src/Player/Gear/ArmoryTexture/";
	m_ArmoryBackTex.Load(path + "armory.png");
	m_gearNullTex.Load(path + "gearNull.png");
	m_gearFrameTex.Load(path + "gearFrame.png");
	m_playerTex.Load(path + "player1.png");
	m_armoryPos = { 0,0 };
	for (int i = 0; i < m_GearNum; i++)
	{
		m_bGearOpen[i] = false;
		m_gearTex[i].Load(path + "gear" + std::to_string(i+1) + ".png");
	}
}

void C_Armory::Update()
{
	DisplayArmory();
	if (!m_bArmoryOpen)return;
	

	MatrixUpdate();
}

void C_Armory::Draw()
{
	if(!m_bArmoryOpen) return;
	//枠
	SHADER.m_spriteShader.SetMatrix(m_Mat);
	Math::Rectangle srcRect = { 0,0,960,576 };	//テクスチャ座標
	Math::Color color = { 1,1,1,1 };
	SHADER.m_spriteShader.DrawTex(&m_ArmoryBackTex, 0, 0, &srcRect, &color);

	//プレイヤー
	SHADER.m_spriteShader.SetMatrix(m_playerMat);
	srcRect =  { 0, 600, 600, 600 };	//テクスチャ座標
	color = { 1,1,1,1 };
	SHADER.m_spriteShader.DrawTex(&m_playerTex, 0, 0, &srcRect, &color);

	//ギアA
	if (!m_bGearOpen[m_gearAIndex]|| m_gearAIndex < 0 || m_gearAIndex >= m_GearNum) {
		SHADER.m_spriteShader.SetMatrix(m_gearAMat);
		srcRect = { 0, 0, 56, 56 };	//テクスチャ座標
		SHADER.m_spriteShader.DrawTex(&m_gearNullTex, 0, 0, &srcRect, &color);
	}
	else
	{
		SHADER.m_spriteShader.SetMatrix(m_gearAMat);
		srcRect = { 0, 0, 56, 56 };	//テクスチャ座標
		SHADER.m_spriteShader.DrawTex(&m_gearTex[m_gearAIndex], 0, 0, &srcRect, &color);
	}
	SHADER.m_spriteShader.SetMatrix(m_gearAMat);
	srcRect = { 0, 0, 64, 64 };	//テクスチャ座標
	SHADER.m_spriteShader.DrawTex(&m_gearFrameTex	, 0, 0, &srcRect, &color);

	//ギアB
	if (!m_bGearOpen[m_gearBIndex]|| m_gearBIndex < 0 || m_gearBIndex >= m_GearNum) {
		SHADER.m_spriteShader.SetMatrix(m_gearBMat);
		srcRect = { 0, 0, 56, 56 };	//テクスチャ座標
		SHADER.m_spriteShader.DrawTex(&m_gearNullTex, 0, 0, &srcRect, &color);
	}
	else
	{
		SHADER.m_spriteShader.SetMatrix(m_gearBMat);
		srcRect = { 0, 0, 56, 56 };	//テクスチャ座標
		SHADER.m_spriteShader.DrawTex(&m_gearTex[m_gearBIndex], 0, 0, &srcRect, &color);
	}
	SHADER.m_spriteShader.SetMatrix(m_gearBMat);
	srcRect = { 0, 0, 64, 64 };	//テクスチャ座標
	SHADER.m_spriteShader.DrawTex(&m_gearFrameTex, 0, 0, &srcRect, &color);
}

void C_Armory::Release()
{
	m_ArmoryBackTex.Release();
	m_playerTex.Release();
	for (int i = 0; i < m_GearNum; i++)
	{
		m_gearTex[i].Release();
	}
	m_gearNullTex.Release();
	m_gearFrameTex.Release();
}

void C_Armory::DisplayArmory()
{
	m_keyDownCount++;
	if (GetAsyncKeyState(VK_TAB) & 0x8000 && !m_kyDown) {

		m_bArmoryOpen = !m_bArmoryOpen;
		m_kyDown = true;
		m_keyDownCount = 0;
	}
	else if(m_keyDownCount > 10){
		m_kyDown = false;
	}
}

void C_Armory::MatrixUpdate()
{
	//行列合成
	//枠用
	m_transMat = Math::Matrix::CreateTranslation(
		m_armoryPos.x, m_armoryPos.y, 0);
	m_scaleMat = Math::Matrix::CreateScale(1.0, 1.0, 1.0f);
	m_Mat = m_scaleMat * m_transMat;

	//プレイヤー用
	m_transMat = Math::Matrix::CreateTranslation(
		m_playerPos.x, m_playerPos.y, 0);
	m_scaleMat = Math::Matrix::CreateScale(1.0 / 2.5, 1.0 / 2.5, 1.0f);
	m_playerMat = m_scaleMat * m_transMat;

	//ギアA用
	m_transMat = Math::Matrix::CreateTranslation(
		m_gearAPos.x, m_gearAPos.y, 0);
	m_scaleMat = Math::Matrix::CreateScale(1.0, 1.0, 1.0f);
	m_gearAMat = m_scaleMat * m_transMat;

	//ギアB用
	m_transMat = Math::Matrix::CreateTranslation(
		m_gearBPos.x, m_gearBPos.y, 0);
	m_scaleMat = Math::Matrix::CreateScale(1.0, 1.0, 1.0f);
	m_gearBMat = m_scaleMat * m_transMat;
}
