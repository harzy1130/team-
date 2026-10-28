#pragma once
class C_PlayerBase;
class EnemyBase;
class C_Effect
{
public:
	C_Effect() {};
	~C_Effect() {};

	void InitPlayer(C_PlayerBase* a_pPlayer);
	void InitEnemy(EnemyBase* a_pEnemy);
	void UpdatePlayer(C_PlayerBase* a_pPlayer);
	void UpdateEnemy(EnemyBase* a_pEnemy);
	void Draw();

	bool GetPoison(){ return m_bPoison; }
	bool GetCombustion(){ return m_bCombustion; }
	bool GetSutan(){ return m_bSutan; }
	bool GetSuppress(){ return m_bSuppress; }
	bool GetAtkUp(){ return m_bAtkUp; }
	bool GetDefUp(){ return m_bDefUp; }

	//引数に時間を秒数で入れるとその秒数だけ効果が持続する
	void SetPoison(float a_time) { m_bPoison = true, m_poisonTime = a_time;m_poisonCount = 0; }
	void SetCombustion(float a_time) { m_bCombustion = true, m_combustionTime = a_time; m_combustionCount = 0; }
	void SetSutan(float a_time) { m_bSutan = true, m_sutanTime = a_time; }
	void SetSuppress(float a_time) { m_bSuppress = true, m_suppressTime = a_time; }
	void SetAtkUp(float a_time) { m_bAtkUp = true, m_atkUpTime = a_time; }
	void SetDefUp(float a_time) { m_bDefUp = true, m_defUpTime = a_time; }
	void SetAtkDown(float a_time) { m_bAtkDown = true, m_atkDownTime = a_time; }
	void SetDefDown(float a_time) { m_bDefDown = true, m_defDownTime = a_time; }
	void SetBaseAtk(float a_atk) { m_baseAtk = a_atk; }
	void SetBaseDef(float a_def) { m_baseDef = a_def; }

private:
	//元のステータスを保存する変数
	float m_baseAtk;
	float m_baseDef;

	bool m_bPoison;		//毒
	int m_poisonCount;	//ダメージ用のカウント変数
	float m_poisonTime;//毒の持続時間

	bool m_bCombustion; //炎上
	int m_combustionCount;
	float m_combustionTime;

	bool m_bSutan;		//スタン
	float m_sutanTime;

	bool m_bSuppress;	//スキル封印
	float m_suppressTime;

	bool m_bAtkUp;		//攻撃力上昇
	float m_atkUpTime;

	bool m_bDefUp;		//防御力上昇
	float m_defUpTime;

	bool m_bAtkDown;	//攻撃力低下
	float m_atkDownTime;

	bool m_bDefDown;	//防御力低下
	float m_defDownTime;

};
