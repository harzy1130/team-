#pragma once
#include"Constants/Constants.h"
#include"HitBase.h"
//==========================================================================================================//
//									当たり判定の使い方について												//
//はじめに																									//
//	前提として、当たり判定はHitBaseクラスを継承したクラスに対して使用する。									//
//	現状はプレイヤー、敵、ボスの3種類がHitBaseを継承している。												//
//  武器や弾などは現状では対応しきれていないため、別クラスで作成し、HitBaseを継承させる必要がある。			//
//	使うときは当たる側（攻撃側）が使用し、基本的には相手に対しての処理を行う。								//
//　例：PlayerにBossに対しての判定を持たせる場合は、IfHitBoss関数内にはBossに対しての処理を記述する。		//
//																											//
//使い方																									//
// 	1.当たり判定を持たせたいクラスにHitBaseを継承させる。													//
//																											//
//  2.hit.hをインクルードし、C_Hitの前方宣言とクラスのポインタを作成する。									//
// 																											//
//	3.当たり判定を持たせたいクラスのInit関数の引数でC_Hitのポインタを受け取り、関数内でSetHitClassを使用	//
//	　し、ポインタをセットする。また、当たり判定のためのHitBox情報はHitBoxEditorで作った.csvをLoadHitBox	//
//    で読み込むことを推奨する。大きさを変更する場合はLoadHitBox実行前にHitBaseのm_scaleに入れること。		//
//																											//
//  4.当たり判定を持たせたいクラスのUpdate関数内で、C_Hitの～～CheckHit関数を使用する。						//
//	　引数にはthisを渡す。戻り値はbool型で、当たった場合はtrue、当たらなかった場合はfalseが返る。			//
//																											//
//  5.当たった場合の処理を書くためのIfHit～～を当たり判定を行うクラスに追加し、関数内に処理を記述する。		//
//	  HitBase内の関数と同じ名前だった場合は自動で呼び出されるが名前が違った場合は、呼び出されないので		//
//	　このヘッダーから関数名をコピペすることを推奨する。													//
//																											//
//	6.当たり判定を行うと、当たった相手のアドレスを格納し、同じ相手とは判定をしなくなるので、任意のタイミ	//
//  　ングでResetAlreadyHit関数を呼び出す必要がある。呼び出すタイミングは、攻撃が終わったタイミングや、		//
//    攻撃開始前に呼び出すのが望ましい。																	//
//==========================================================================================================//
class C_Map;
class EnemyManager;
class C_BossBase;
class C_PlayerBase;
class C_Hit
{
public:
	C_Hit() = default;
	~C_Hit() = default;
	// 初期化関数
	void Init(C_Map* a_pMap, EnemyManager* a_pEnemyManager, C_BossBase* a_pBoss, C_PlayerBase* a_pPlayer);
	void SetMapClass(C_Map* a_pMap) { m_pMap = a_pMap; }
	void SetEnemyManager(EnemyManager* a_pEnemyManager) { m_pEnemyManager = a_pEnemyManager; }
	void SetBossClass(C_BossBase* a_pBoss) { m_pBoss = a_pBoss; }
	void SetPlayerClass(C_PlayerBase* a_pPlayer) { m_pPlayer = a_pPlayer; }

	// 引数に入れたHitBaseを持つポインタにMapへの当たり判定を持たせる。
	void MapHitCheck(HitBase* a_pHitBase);
	// 引数に入れたHitBaseを持つポインタに敵への当たり判定を持たせる。
	bool EnemyHitCheck(HitBase* a_pHitBase);
	// 引数に入れたHitBaseを持つポインタにボスへの当たり判定を持たせる。
	bool BossHitCheck(HitBase* a_pHitBase);
	// 引数に入れたHitBaseを持つポインタにプレイヤーへの当たり判定を持たせる。
	bool PlayerHitCheck(HitBase* a_pHitBase);
	// 円と円の当たり判定を行う
	bool CircleHit(Math::Vector2 a_pos1, float a_radius1,Math::Vector2 a_pos2, float a_radius2);
	// 円と矩形の当たり判定を行う
	bool RectCircleHit(Math::Vector2 a_rectPos, RECT a_rect, Math::Vector2 a_circlePos, float a_circleRadius);
	// 矩形と矩形の当たり判定を行う。
	bool RectHit(Math::Vector2 a_pos1, RECT a_rect1, Math::Vector2 a_pos2, RECT a_rect2);
    // 距離・上下の重なり・向きだけを調べ、攻撃タイミングやダメージは呼び出し側で管理する。
    bool CheckAttackHit(const HitBase& attacker, const HitBase& target,
        float horizontalReach, int facingDirection = 0) const;
private:
	RECT CreateRectPos(float a_halfWidth, float a_halfHeight, Math::Vector2 a_pos);
	bool RectInPos(RECT a_rect, Math::Vector2 a_pos);
	bool CheckHitBox(HitBase* a_pHitBase, HitBase* a_pTarget);
	C_Map* m_pMap = nullptr;
	EnemyManager* m_pEnemyManager = nullptr;
	C_BossBase* m_pBoss = nullptr;
	C_PlayerBase* m_pPlayer = nullptr;
};