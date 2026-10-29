#pragma once
#include"Constants/Constants.h"
#include"Skill/SkillBase.h"
#include"Camera/Camera.h"
#include"Hit/HitBase.h"

class C_Cursor;
class C_PlayerBase : public HitBase
{
public:
	C_PlayerBase() {};
	~C_PlayerBase() {};

	virtual void Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Cursor*a_pCursor);
	virtual void Update();
	virtual void Draw();

    // 被ダメージの共通処理。防御力・無敵時間・HP 減少・死亡を処理する。
    bool TakeDamage(float rawDamage);
    bool IsDamageInvincible() const { return m_damageInvincibleFrames > 0; }
	// 敵と当たった時に行う処理を記述する場所
	void IfHitEnemy(EnemyBase* a_pEnemy);

	//ゲッター
	//生存
	virtual bool GetAFlg()final { return m_bAlive; }
	//移動
	virtual Math::Vector2 GetSkillMove()final { return m_skillMove; }
	virtual bool GetJumpFlg()final { return m_bJump; }
	virtual float GetScaleX()final { return m_scaleX; }
	//基本ステータス
	virtual float GetHP()final { return m_HP; }
	virtual float GetMAXHP()final { return m_MAXHP; }
	virtual float GetDEF()final { return m_DEF; }
	virtual float GetATK()final { return m_ATK; }
	virtual float GetAS()final { return m_AS; }
	//特殊ステータス
	virtual float GetLifStl()final { return m_LifStl; }
	virtual float GetMS()final { return m_MSUP; }
	virtual float GetCrtDmg()final { return m_CrtDmgP; }
	virtual float GetCrtRate()final { return m_CrtRate; }
	virtual float GetSkillHaste()final { return m_SkillHaste; }

	//セッター
	//生存
	virtual void SetAFlg(bool a_flg)final { m_bAlive = a_flg; }
	//画像
	virtual void SetTex(KdTexture* a_tex)final { m_pTex = a_tex; }
	virtual void SetAtkTex(KdTexture* a_tex)final { m_pAtkTex = a_tex; }

	//移動
	virtual void SetPosX(float a_posX)final { m_pos.x = a_posX; }
	virtual void SetPosY(float a_posY)final { m_pos.y = a_posY; }
	virtual void SetMoveX(float a_moveX)final { m_move.x = a_moveX; }
	virtual void SetMoveY(float a_moveY)final { m_move.y = a_moveY; }
	virtual void SetSkillMoveX(float a_moveX) { m_skillMove.x = a_moveX; }
	virtual void SetSkillMoveY(float a_moveY) { m_skillMove.y = a_moveY; }
	virtual void SetJumpFlg(bool a_flg)final { m_bJump = a_flg; }

	//基本ステータス
	virtual void SetHP(float a_HP)final { m_HP = a_HP; }
	virtual void SetMAXHP(float a_MAXHP)final { m_MAXHP = a_MAXHP; }
	virtual void SetDEF(float a_DEF)final { m_DEF = a_DEF; }
	virtual void SetATK(float a_ATK)final { m_ATK = a_ATK; }
	virtual void SetAS(float a_AS)final { m_AS = a_AS; }
	//特殊ステータス
	virtual void SetLifStl(float a_LS)final { m_LifStl = a_LS; }
	virtual void SetMS(float a_MS)final { m_MSUP = a_MS; }
	virtual void SetHPRegene(float a_HPR)final { m_HPRegene = a_HPR; }
	virtual void SetCrtDmg(float a_CD)final { m_CrtDmgP = a_CD; }
	virtual void SetCrtRate(float a_CR)final { m_CrtRate = a_CR; }
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
	float m_speed;
	float m_scaleX = 1;
	bool m_bJump;
	const float m_JumpPower = 20.0f;

	//画像用
	KdTexture* m_pTex;
	Math::Matrix m_mat;
	Math::Matrix m_transMat;
	Math::Matrix m_scaleMat;



	//基礎ステータス
	int m_Level;
	float m_HP;		//現在体力
	float m_MAXHP;	//最大体力
	float m_DEF;	//防御力
	float m_ATK;	//攻撃力
	float m_AS;		//攻撃速度

	//特殊ステータス
	float m_LifStl;		//ライフスティール
	float m_MSUP;		//移動速度
	float m_HPRegene;	//ライフリジェネ
	float m_CrtDmgP;	//クリティカルダメージ
	float m_CrtRate;	//クリティカル率
	float m_SkillHaste;	//スキルヘイスト
	float m_IncreasedASP;//増加攻撃速度

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

	
	//参照するクラスのポインタ
	C_Hit* m_pHit;
	C_Camera* m_pCamera;
	C_Cursor* m_pCursor;

	//タイマー変数
	float m_HPRegeneCD=0;
	float m_AttackDelay;

	//計算関数
	virtual float DefCalc(float a_DMG, float a_DEF,float a_penet)final;
	virtual void HPRegeneCalc()final;
	virtual float CrtDmgCalc(float a_DMG, float a_CrtDmg, float a_CrtRate)final;
	virtual float MoveSpeedCalc(float a_speed, float a_MS)final;
	virtual float AtkSpeedCalc()final;

	//操作
	virtual void PlayerInput();

	//行列合成
	virtual void CalcMat()final;

	//攻撃
	virtual void PlayerAtk();
	virtual void InitAtk();
	virtual void UpdateAtk();
	virtual void DrawAtk();

	//スキル用
	C_Skill m_skill;
};
