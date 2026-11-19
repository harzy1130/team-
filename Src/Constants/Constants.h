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
	gate1 = 50,
	gate2 = 51,
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

struct S_Status
{
	//通常ステータス
	int maxHp;
	float def;
	float atk;
	//特殊ステータス
	float lifStl;		//ライフスティール
	float msUP;		//移動速度
	float hpRegene;	//ライフリジェネ
	float crtDmgP;	//クリティカルダメージ
	float crtRate;	//クリティカル率
	float skillHaste;	//スキルヘイスト
	float increasedASP;//増加攻撃速度
	S_Status operator+(const S_Status& rhs) const
	{
		return {
			maxHp + rhs.maxHp,
			def + rhs.def,
			atk + rhs.atk,
			lifStl + rhs.lifStl,
			msUP + rhs.msUP,
			hpRegene + rhs.hpRegene,
			crtDmgP + rhs.crtDmgP,
			crtRate + rhs.crtRate,
			skillHaste + rhs.skillHaste,
			increasedASP + rhs.increasedASP
		};
	}
	S_Status operator+=(const S_Status& rhs)
	{
		maxHp += rhs.maxHp;
		def += rhs.def;
		atk += rhs.atk;
		lifStl += rhs.lifStl;
		msUP += rhs.msUP;
		hpRegene += rhs.hpRegene;
		crtDmgP += rhs.crtDmgP;
		crtRate += rhs.crtRate;
		skillHaste += rhs.skillHaste;
		increasedASP += rhs.increasedASP;
		return *this;
	}
};
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

struct S_ObbObjects {
	Math::Vector2 center;
	Math::Vector2 halfSize;
	float angle;
};
//カメラ移動定数==============================

static const float CAMERA_SPEED = 0.1f;

//UI用定数====================================

static const Math::Color VALID_COLOR = Math::Color{ 1.0, 1.0, 1.0, 1.0f };
static const Math::Color INVALID_COLOR = Math::Color{ 0.4, 0.4, 0.4, 1.0f };

static const int lightNum = 8;
struct S_Light
{
	Math::Vector2 scale = Math::Vector2{1.0f, 1.0f};
	float deg;
	Math::Color color;
	Math::Matrix mat;
};
struct S_Grow
{
	S_Light light[lightNum];
	Math::Vector2 pos;
	float size;
	float destSize;
};

struct S_StatusChip
{
	KdTexture* pTex;
	float* maxTime;
	float* time;
};