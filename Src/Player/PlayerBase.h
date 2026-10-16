#pragma once
#include"Constans/Constants.h"
class C_PlayerBase
{
public:
	C_PlayerBase() {};
	~C_PlayerBase() {};

	virtual void Init();
	virtual void Update();
	virtual void Draw(Math::Vector2 a_pos);

	//ゲッター
	//生存
	virtual bool getAFlg()final { return m_bAlive; }
	//画像サイズ
	virtual float getTexSize()final { return m_TexSize; }
	//移動
	virtual Math::Vector2 getPos()final { return m_pos; }
	virtual Math::Vector2 getMove()final { return m_move; }
	virtual bool getJumpFlg()final { return m_jumpFlg; }
	//基本ステータス
	virtual float getHP()final { return m_HP; }
	virtual float getMAXHP()final { return m_MAXHP; }
	virtual float getDEF()final { return m_DEF; }
	virtual float getATK()final { return m_ATK; }
	virtual float getAS()final { return m_AS; }
	//特殊ステータス
	virtual float getLifStl()final { return m_LifStl; }
	virtual float getMS()final { return m_MSUP; }
	virtual float getCrtDmg()final { return m_CrtDmgP; }
	virtual float getCrtRate()final { return m_CrtRate; }
	//当たり判定用
	virtual S_HitBase getHitBase()final { return m_hitBase; }

	//セッター
	//生存
	virtual void setAFlg(bool a_flg)final { m_bAlive = a_flg; }
	//画像
	virtual void setTex(KdTexture* a_tex)final { m_pTex = a_tex; }
	//移動
	virtual void setPosX(float a_posX)final { m_pos.x = a_posX; }
	virtual void setPosY(float a_posY)final { m_pos.y = a_posY; }
	virtual void setMoveX(float a_moveX)final { m_move.x = a_moveX; }
	virtual void setMoveY(float a_moveY)final { m_move.y = a_moveY; }
	virtual void setJumpFlg(bool a_flg)final { m_jumpFlg = a_flg; }

	//基本ステータス
	virtual void setHP(float a_HP)final { m_HP = a_HP; }
	virtual void setMAXHP(float a_MAXHP)final { m_MAXHP = a_MAXHP; }
	virtual void setDEF(float a_DEF)final { m_DEF = a_DEF; }
	virtual void setATK(float a_ATK)final { m_ATK = a_ATK; }
	virtual void setAS(float a_AS)final { m_AS = a_AS; }
	//特殊ステータス
	virtual void setLifStl(float a_LS)final { m_LifStl = a_LS; }
	virtual void setMS(float a_MS)final { m_MSUP = a_MS; }
	virtual void setHPRegene(float a_HPR)final { m_HPRegene = a_HPR; }
	virtual void setCrtDmg(float a_CD)final { m_CrtDmgP = a_CD; }
	virtual void setCrtRate(float a_CR)final { m_CrtRate = a_CR; }
	//当たり判定用
	virtual void setHitBase(S_HitBase a_hitbase) { m_hitBase = a_hitbase; }


protected:
	//生存
	bool m_bAlive;

	//移動用
	Math::Vector2 m_pos;
	Math::Vector2 m_move;
	float m_speed;
	float m_scaleX=1;
	bool m_jumpFlg;
	const float m_Gravity = 1.0f;//重力
	const float m_JumpPower = 20.0f;

	//画像用
	KdTexture* m_pTex;
	Math::Matrix m_mat;
	Math::Matrix m_transMat;
	Math::Matrix m_scaleMat;
	int m_TexSize;
	int m_TexRadius;

	//基礎ステータス
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

	//当たり判定用
	S_HitBase m_hitBase;


	//タイマー変数
	float m_HPRegeneCD=0;

	//計算関数
	virtual float DefCalc(float a_DMG, float a_DEF,float a_penet)final;
	virtual void HPRegeneCalc()final;
	virtual float CrtDmgCalc(float a_DMG, float a_CrtDmg, float a_CrtRate)final;
	virtual float MoveSpeedCalc(float a_speed, float a_MS)final;

	//操作
	virtual void PlayerInput();

	//hitBaseから値移し
	virtual void HitBaseResult();
	virtual void HitBaseSet();

	//行列合成
	virtual void MatCalc()final;

};
