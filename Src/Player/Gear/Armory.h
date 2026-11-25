#pragma once
#include"GearBase.h"
//#include<memory>

class C_Cursor;
class C_PlayerBase;
class C_Armory
{
public:
	C_Armory() {};
	~C_Armory() {};

	void Init();
	void Update();
	void Draw();
	void Release();

	void DisplayArmory();

	void MatrixUpdate();
		
	bool GetArmoryOpen() { return m_bArmoryOpen; }
	//ギアのインデックス取得
	int GetGearAIndex() { return m_gearAIndex; }
	int GetGearBIndex() { return m_gearBIndex; }

private:
	
	bool m_bArmoryOpen = false;
	bool m_kyDown = false;
	int m_keyDownCount = 0;

	//装備画面
	Math::Vector2 m_armoryPos;
	KdTexture m_ArmoryBackTex;
	KdTexture m_gearFrameTex;
	Math::Matrix m_transMat;
	Math::Matrix m_scaleMat;
	Math::Matrix m_Mat;

	//プレイヤー情報
	Math::Matrix m_playerMat;
	Math::Vector2 m_playerPos = { -250, 50 };
	KdTexture m_playerTex;
	//ギアA
	int m_gearAIndex = 0;
	Math::Matrix m_gearAMat;
	Math::Vector2 m_gearAPos = { -250, 0 };
	std::unique_ptr<C_GearBase> m_pGearA;
	//ギアB
	int m_gearBIndex = 0;
	Math::Matrix m_gearBMat;
	Math::Vector2 m_gearBPos = { -250, -50 };
	std::unique_ptr<C_GearBase> m_pGearB;

	//装備管理
	static const int m_GearNum = 5;
	bool m_bGearOpen[m_GearNum];
	KdTexture m_gearTex[m_GearNum];
	KdTexture m_gearNullTex;

	//参照クラスポインタ
	C_PlayerBase* m_pPlayer = nullptr;
	C_Cursor* m_pCursor = nullptr;
};
