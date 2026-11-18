#pragma once
class C_PlayerBase;
class EnemyBase;
class C_UIManager;
class C_Effect
{
public:
	C_Effect() {};
	~C_Effect() {
		m_poisonIconTex.Release();
	};

	void InitPlayer(C_PlayerBase* a_pPlayer, C_UIManager* a_pUIManager);
	void InitEnemy(EnemyBase* a_pEnemy);
	void UpdatePlayer();
	void UpdateEnemy();
	void Draw();
	void EffectTest();

	bool GetPoison(){ return m_bPoison; }
	bool GetCombustion(){ return m_bCombustion; }
	bool GetSutan(){ return m_bSutan; }
	bool GetSuppress(){ return m_bSuppress; }
	bool GetAtkUp(){ return m_bAtkUp; }
	bool GetDefUp(){ return m_bDefUp; }

	//状態異常のダメージタイミング取得用の関数
	float GetPoisonTime() { return m_poisonCount; }
	float GetCombustionTime() { return m_combustionCount; }

	float GetPoinsonRatio() { return m_poisonTime / m_poisonMaxTime; }

	//引数に時間を秒数で入れるとその秒数だけ効果が持続する
	void SetPoison(float a_time);
	void SetCombustion(float a_time) { m_bCombustion = true; m_combustionTime = a_time; }
	void SetSutan(float a_time) { m_bSutan = true; m_sutanTime = a_time; }
	void SetSuppress(float a_time) { m_bSuppress = true; m_suppressTime = a_time; }

	//引数に時間(秒数)と上昇する値を入れるとその秒数だけ効果が持続する
	void SetAtkUp(float a_time, int a_atkUp);
	void SetDefUp(float a_time, int a_defUp);
	void SetAtkDown(float a_time, int a_atkDown);
	void SetDefDown(float a_time, int a_defDown);

	//元のステータスを保存する変数をセットする関数
	void SetBaseAtk(float a_atk) { m_baseAtk = a_atk; }
	void SetBaseDef(float a_def) { m_baseDef = a_def; }

	//攻撃力上昇倍率を返す関数
	float GetAtkRatio() { return (100+m_atkUpP - m_atkDownP) / 100.0f; }
	float GetDefRatio() { return (100 + m_defUpP - m_defDownP) / 100.0f; }

private:
	//元のステータスを保存する変数
	float m_baseAtk;
	float m_baseDef;

	KdTexture m_poisonIconTex;
	bool m_bPoison;		//毒
	int m_poisonCount;	//ダメージ用のカウント変数
	float m_poisonTime;//毒の持続時間
	float m_poisonMaxTime; //現在の毒の持続時間の最大値

	bool m_bCombustion; //炎上
	int m_combustionCount;
	float m_combustionTime;

	bool m_bSutan;		//スタン
	float m_sutanTime;

	bool m_bSuppress;	//スキル封印
	float m_suppressTime;

	bool m_bAtkUp;		//攻撃力上昇(割合)
	float m_atkUpTime;
	int m_atkUpP;

	bool m_bDefUp;		//防御力上昇(割合)
	float m_defUpTime;
	int m_defUpP;

	bool m_bAtkDown;	//攻撃力低下（割合）
	float m_atkDownTime;
	int m_atkDownP;

	bool m_bDefDown;	//防御力低下（割合）
	float m_defDownTime;
	int m_defDownP;

	//プレーヤーに渡す値


	//参照クラスのポインタ
	C_PlayerBase* m_pPlayer;
	EnemyBase* m_pEnemy;
	C_UIManager* m_pUIManager;

};
