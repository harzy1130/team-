#pragma once
#include"Pch.h"

class C_Camera;
class C_NumberUI
{
public:
	C_NumberUI() {};
	~C_NumberUI() {};

	void Init(C_Camera* a_pCamera);
	void Update();
	void Draw();
	void Release();
	void Move();

	//数字の生成
	void Generate(int a_number, Math::Vector2 a_pos,Math::Vector2 a_moveSpeed, float a_size,float a_angle, Math::Color a_color, float a_lifespan,bool a_movingDirectionX);
	
	//ゲッター
	Math::Vector2 GetPos() { return m_pos; }
	Math::Vector2 GetDrawPos() { return m_drawPos; }
	float GetAngle() { return m_angle; }
	bool GetDrawFlg() { return m_drawFlg; }

	//セッター
	void SetPos(Math::Vector2 a_pos) { m_pos = a_pos; }
	void SetAngle(float a_angle) { m_angle = a_angle; }
	void SetTexture(KdTexture* a_pTex) { m_pTexture = a_pTex; }


private:
	
	//数字のテクスチャ
	KdTexture m_numberTex;

	//数字の描画情報
	bool m_drawFlg = false;	//描画するかどうか
	int m_number;	//表示する数字
	int m_thousand;	//千の位
	int m_hundred;	//百の位
	int m_ten;		//十の位
	int m_one;		//一の位
	bool m_minus;//負の値かどうか
	//Math::Vector2 m_minusGap;	//マイナスの間隔
	Math::Vector2 m_thousandGap;//数字の間隔(千)
	Math::Vector2 m_hundredGap;	//数字の間隔(百)
	Math::Vector2 m_tenGap;	//数字の間隔(十)
	Math::Vector2 m_oneGap;	//数字の間隔(一)

	//数字の描画位置
	Math::Vector2 m_pos;
	Math::Vector2 m_drawPos;
	Math::Vector2 m_moveSpeed;
	bool m_movingDirectionX;//移動方向(x)true:+方向　,false:ー方向
	float m_moveAngle;//sinカーブ用
	float m_size;//数字の大きさ
	Math::Color m_color;//数字の色
	float m_lifespan;//有効期間
	float m_lifespanTimer;//有効期間のタイマー
	static const int m_numbersTexWidth = 28;	//数字のテクスチャの幅

	//画像用変数
	Math::Matrix m_minusMat;//マイナスの記号用
	Math::Matrix m_thousandMat;//千の位
	Math::Matrix m_hundredMat;//百の位
	Math::Matrix m_tenMat;//十の位
	Math::Matrix m_oneMat;//一の位
	
	Math::Matrix m_transMat;
	Math::Matrix m_scaleMat;
	Math::Matrix m_rotMat;
	KdTexture* m_pTexture;
	float m_angle;

	//参照クラスポインタ
	C_Camera* m_pCamera = nullptr;

};

