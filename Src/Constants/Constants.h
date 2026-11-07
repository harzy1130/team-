#pragma once
//画面定数=======================================
const float SCREEN_HEIGHT = 720;
const float SCREEN_WIDTH  = 1280;
const float TOP_EDGE      = SCREEN_HEIGHT  / 2;
const float BOTTOM_EDGE   = -SCREEN_HEIGHT / 2;
const float RIGHT_EDGE    = SCREEN_WIDTH   / 2;
const float LEFT_EDGE     = -SCREEN_WIDTH  / 2;

//GameState定数=================================
const float SCENETRANS_TIME = 60;
const float LOAD_TIME = 90;

const enum GameState
{
	Title = 0,
	Select = 1,
	Game = 2,
	Result = 3
};

struct S_RenderTarget
{
	float x;
	float y;
	float scaleX;
	float scaleY;
	float scale;
	Math::Matrix mat;
	Math::Matrix transMat;
	Math::Matrix scaleMat;
};

//マップ定数====================================
static const int  MAP_HEIGHT  = 12;
static const int  MAP_WIDTH   = 64;
const int		  CHIP_SIZE   = 64;
const int		  CHIP_RADIUS = CHIP_SIZE / 2;
const std::string NEXT_ROW    = "NEXT_ROW";

struct S_ChipData
{
	Math::Matrix mat;
	Math::Vector2 pos;
	Math::Vector2 draw;
	int data;
	bool collisionFlg;
};

const enum MapChip
{
	dirt = 1,
	glass = 2,
	stone = 3,
	gate1 = 10,
	gate2 = 11,
};

const enum StageNum
{
	stageSelect = 0,
	stage1      = 1,
	stage2      = 2,
	Test        = 10,
};

//プレイヤー定数================================
const float      GRAVITY    = 1.0f;
const float      JUMP_POWER = 20.0f;
static const int PTEX_SIZE  = 64;
static const int PTEX_RADIUS= PTEX_SIZE / 2;

//ボス定数======================================



//HitBase定数=================================
struct S_HitBase
{
	RECT rect;
	bool hitTop;
	bool hitBottom;
	bool hitLeft;
	bool hitRight;
	Math::Vector2 pos;
	Math::Vector2 move;
	float radius;
};

struct S_HitRect
{
	bool left;
	bool top;
	bool right;
	bool bottom;
};

struct S_CircleObjects {
	Math::Vector2 gap;
	float radius;
};

struct S_RectObjects {
	Math::Vector2 gap;
	RECT rect;
};
//カメラ移動定数==============================

static const float CAMERA_SPEED = 0.1f;

//パーティクル定数============================

struct S_ParticleObjects
{
	Math::Vector2 pos;
	Math::Vector2 move;
	Math::Vector2 draw;
	Math::Vector2 scale;
	Math::Color	  color;
	Math::Matrix  mat;
	Math::Matrix  sMat;
	Math::Matrix  rMat;
	Math::Matrix  tMat;
	float		  size;
	float		  m_lifeSpan;
	float		  m_alpha;
	int			  data;
};
