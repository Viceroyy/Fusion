#pragma once
#include "../../SDK/SDK.h"

class CEnginePrediction
{
private:
	CMoveData m_MoveData = {};

private:
	void Simulate(CTFPlayer* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd);

	int m_nOldTickCount = 0;
	int nOldTickBase = 0;
	float m_fOldCurrentTime = 0.f;
	float m_fOldFrameTime = 0.f;

	bool bSimulated = false;

public:
	void Start(CTFPlayer* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd);
	void End(CTFPlayer* pLocal, CUserCmd* pCmd);
};

ADD_FEATURE(CEnginePrediction, EnginePrediction)
