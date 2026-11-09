#pragma once
#include"Constants/Constants.h"
#include"Skill/SkillBase.h"
#include"Camera/Camera.h"
#include"Hit/HitBase.h"
#include"Hit/Hit.h"
#include"Effect/effect.h"
#include "Gear/Gear1.h"
#include "Gear/Gear2.h"

enum E_MoveMode
{
	run = 0,
	stand = 1,
	attacking = 2,
	defense = 3,
};

class C_GearBase;
class C_Cursor;
class C_PlayerBase : public HitBase
{
public:
	C_PlayerBase() {};
	~C_PlayerBase() {};

	virtual void Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Cursor*a_pCursor);
	virtual void Update();
	virtual void Draw();
	virtual void GearUpdate();

    // 被ダメージの共通処理。防御力・無敵時間・HP 減少・死亡を処理する。
    bool TakeDamage(float rawDamage);
    bool IsDamageInvincible() const { return m_damageInvincibleFrames > 0; }
	// 外部からも呼び出す固定アクション
	bool Heal(float a_heal);
	//ゲッター
	//生存
	virtual bool GetAFlg()final { return m_bAlive; }
	//移動
	virtual Math::Vector2 GetSkillMove()final { return m_skillMove; }
	virtual bool GetJumpFlg()final { return m_bJump; }
	virtual float GetScaleX()final { return m_scale.x; }
	virtual S_HitRect GetHitRect()final { return m_bHitRect; }
	//基本ステータス
	virtual float GetHP()final { return m_Hp; }
	virtual float GetMAXHP()final { return m_status.maxHp; }
	virtual float GetDEF()final { return m_status.def; }
	virtual float GetATK()final { return m_status.atk; }
	virtual float GetAS()final { return m_atkSpeed; }
	//特殊ステータス
	virtual float GetLifStl()final { return m_status.lifStl; }
	virtual float GetMS()final { return m_status.msUP; }
	virtual float GetCrtDmg()final { return m_status.crtDmgP; }
	virtual float GetCrtRate()final { return m_status.crtRate; }
	virtual float GetSkillHaste()final { return m_status.skillHaste; }
	//攻撃関係
	virtual bool GetAtkFlg()final { return m_bAtk; }
	//スキル関係
	virtual float GetSkillCD()final { return m_skill.GetSkillCD(); }
	virtual float GetSkillCDMax()final { return m_skill.GetSkillCDMax(); }
	virtual float GetSkillCalcCD()final { return m_skill.GetSkillCalcCD(this); }
	virtual KdTexture* GetSkillIconTex()final { return m_pSkillIconTex; }
	//gear関係
	virtual const C_GearBase* GetGearA()final { return m_pGearA.get(); }
	virtual const C_GearBase* GetGearB()final { return m_pGearB.get(); }

	//セッター
	//生存
	virtual void SetAFlg(bool a_flg)final { m_bAlive = a_flg; }
	//画像
	virtual void SetTex(KdTexture* a_tex)final { m_pTex = a_tex; }
	virtual void SetAtkTex(KdTexture* a_tex)final { m_pAtkTex = a_tex; }
	virtual void SetSkillTex(KdTexture* a_tex)final { m_pSkillTex = a_tex; }
	virtual void SetSkillIconTex(KdTexture* a_tex)final { m_pSkillIconTex = a_tex; }

	//移動
	virtual void SetPosX(float a_posX)final { m_pos.x = a_posX; }
	virtual void SetPosY(float a_posY)final { m_pos.y = a_posY; }
	virtual void SetMoveX(float a_moveX)final { m_move.x = a_moveX; }
	virtual void SetMoveY(float a_moveY)final { m_move.y = a_moveY; }
	virtual void SetSkillMoveX(float a_moveX) { m_skillMove.x = a_moveX; }
	virtual void SetSkillMoveY(float a_moveY) { m_skillMove.y = a_moveY; }
	virtual void SetJumpFlg(bool a_flg)final { m_bJump = a_flg; }

	//基本ステータス
	virtual void SetHP(float a_HP)final { m_Hp = a_HP; }
	virtual void SetMAXHP(float a_MAXHP)final { m_status.maxHp = a_MAXHP; }
	virtual void SetDEF(float a_DEF)final { m_status.def = a_DEF; }
	virtual void SetATK(float a_ATK)final { m_status.atk = a_ATK; }
	virtual void SetAS(float a_AS)final { m_atkSpeed = a_AS; }
	//特殊ステータス
	virtual void SetLifStl(float a_LS)final { m_status.lifStl = a_LS; }
	virtual void SetMS(float a_MS)final { m_status.msUP = a_MS; }
	virtual void SetHPRegene(float a_HPR)final { m_status.hpRegene = a_HPR; }
	virtual void SetCrtDmg(float a_CD)final { m_status.crtDmgP = a_CD; }
	virtual void SetCrtRate(float a_CR)final { m_status.crtRate = a_CR; }
	//計算関数
	virtual float CrtDmgCalc()final;
	//参照する外部クラス
	virtual void SetHitClass(C_Hit* a_pHit) final { m_pHit = a_pHit; }
	virtual void SetCameraClass(C_Camera* a_pCamera) final { m_pCamera = a_pCamera; }
	virtual void SetCursorClass(C_Cursor* a_pCursor)final { m_pCursor = a_pCursor; }


protected:
    int m_damageInvincibleFrames = 0;
	//生存
	bool m_bAlive;

	//移動用
	//Math::Vector2 m_pos;
	//Math::Vector2 m_move;
	Math::Vector2 m_skillMove;
	float m_moveSpeed=6.5f;
	float m_speed;
	bool m_bJump;
	const float m_JumpPower = 20.0f;
	float m_moveAnimCount = 0;
	E_MoveMode m_moveMode = E_MoveMode::stand;


	//画像用
	KdTexture* m_pTex;
	Math::Matrix m_mat;
	Math::Matrix m_transMat;
	Math::Matrix m_scaleMat;
	Math::Matrix m_rotMat;
	float m_angle;

	//ステータス
	int m_Level=1;
	int m_Exp = 0;
	int m_NextLvExp = 100;
	int m_ExpUp = 30;
	int m_Hp;		//現在体力
	float m_atkSpeed;//攻撃速度
	S_Status m_playerStatus;
	S_Status m_gearStatus;
	S_Status m_status;

	//基礎ステータス(1レベル時のステータス)
	int m_baseHp=100;
	float m_baseAtk=30;
	float m_baseDef=5;

	//１レベルごとのステータス上昇値
	int m_LevelUpHp = 10;
	float m_LevelUpAtk = 4;
	float m_LevelUpDef = 4;
	float m_LevelUpAtkSpeedP = 0.03f;

	//特殊ステータス

	//通常攻撃
	KdTexture* m_pAtkTex;
	Math::Matrix m_AtkMat;
	Math::Matrix m_AtkTransMat;
	Math::Matrix m_AtkScaleMat;
	Math::Vector2 m_AtkPos;
	float m_AtkCount;
	float m_maxAtkCount;
	float m_atkAnimCount;
	bool m_bAtk;

	//防御用変数
	bool m_bDef;

	//参照するクラスのポインタ
	C_Hit* m_pHit;
	C_Camera* m_pCamera;
	C_Cursor* m_pCursor;

	//タイマー変数
	float m_HPRegeneCD=0;
	float m_AttackDelay;

	//回復等のバッファ
	float m_hpRegeneBuffer = 0;

	//計算関数
	virtual float DefCalc(float a_DMG, float a_DEF,float a_penet)final;
	virtual void HPRegeneCalc()final;
	virtual float MoveSpeedCalc(float a_speed, float a_MS)final;
	virtual float AtkSpeedCalc()final;
	
	//操作
	virtual void PlayerInput();

	//行列合成
	virtual void CalcMat()final;
	virtual void MatrixUpdate()final;

	//アニメーション
	virtual void AnimationUpdate()final;

	//攻撃
	virtual void PlayerAtk();
	virtual void InitAtk();
	virtual void UpdateAtk();
	virtual void DrawAtk();

	//防御
	virtual void PlayerDef();
	virtual	void UpdateDef();

	//経験値処理
	virtual void ExpUpdate()final;

	//ステータス処理
	virtual void StatusUpdate()final;
	virtual void CalcGearStatus()final;

	//スキル用
	KdTexture* m_pSkillTex;
	KdTexture* m_pSkillIconTex;
	C_Skill m_skill;

	//装備用
	std::unique_ptr<C_GearBase> m_pGearA;
	std::unique_ptr<C_GearBase> m_pGearB;
	//状態異常用
	C_Effect m_effect;
};
