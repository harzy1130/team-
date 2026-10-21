#pragma once
struct S_Boss
{
	//ó‘Ô
	bool m_bAlive;
	float m_xScale;
	float m_yScale;
	float m_angle;
	KdTexture* m_pTex;

	//À•W
	Math::Vector2 m_pos;
	Math::Vector2 m_move;
	Math::Vector2 m_draw;

	//s—ñ
	Math::Matrix m_mat;
	Math::Matrix m_tMat;
	Math::Matrix m_sMat;
	Math::Matrix m_rMat;
};

static const float MOVE_SPEED = 10;