#pragma once
//画面定数=======================================
const float SCREEN_HEIGHT = 720;
const float SCREEN_WIDTH  = 1280;
const float TOP_EDGE      = SCREEN_HEIGHT  / 2;
const float BOTTOM_EDGE   = -SCREEN_HEIGHT / 2;
const float RIGHT_EDGE    = SCREEN_WIDTH   / 2;
const float LEFT_EDGE     = -SCREEN_WIDTH  / 2;

//マップ定数====================================
static const int  MAP_HEIGHT  = 12;
static const int  MAP_WIDTH   = 32;
const int		  CTEX_SIZE   = 64;
const int		  CHIP_RADIUS = CHIP_RADIUS / 2;
const std::string NEXT_ROW    = "NEXT_ROW";

//プレイヤー定数================================
const float      GRAVITY    = 1.0f;
const float      JUMP_POWER = 20.0f;
static const int PTEX_SIZE  = 64;

//ボス定数======================================
struct S_Boss
{
	//状態
	bool  bAlive;
	float xScale;
	float yScale;
	float angle;
	KdTexture* pTex;

	//座標
	Math::Vector2 pos;
	Math::Vector2 move;
	Math::Vector2 draw;

	//行列
	Math::Matrix mat;
	Math::Matrix tMat;
	Math::Matrix sMat;
	Math::Matrix rMat;
};
static const float MOVE_SPEED = 10.0f;

//HitBase定数==================
struct S_HitBase
{
	RECT rect;
	bool hitTop;
	bool hitBottom;
	bool hitLeft;
	bool hitRight;
	Math::Vector2 pos;
	Math::Vector2 Move;
	float radius;
};
