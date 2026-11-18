#pragma once
#include"SkillBase.h"
class C_DashSkill: public C_SkillBase
{
public:
	C_DashSkill() {};
	~C_DashSkill() {};
	void Init(C_PlayerBase* a_pPlayer)override;
	void Draw()override;

	//ƒXƒLƒ‹”­“®—p
	void OnceActiveSkill()override;
	void WhileActiveSkill()override;
	void EndActiveSkill()override;

private:

};

