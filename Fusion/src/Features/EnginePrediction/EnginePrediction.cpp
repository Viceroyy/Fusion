#include "EnginePrediction.h"

#include "../TickHandler/TickHandler.h"

void CEnginePrediction::Start(CTFPlayer* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd)
{
	if(!pLocal || !pLocal->IsAlive() || !pWeapon || !pCmd || !I::MoveHelper)
		return;

	m_nOldTickCount = I::GlobalVars->tickcount;
	m_fOldCurrentTime = I::GlobalVars->curtime;
	m_fOldFrameTime = I::GlobalVars->frametime;

	pLocal->SetCurrentCmd(pCmd);
	*G::RandomSeed() = MD5_PseudoRandom(pCmd->command_number) & std::numeric_limits<int>::max();

	I::GlobalVars->curtime = TICKS_TO_TIME(I::GlobalVars->tickcount);
	I::GlobalVars->frametime = I::Prediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL;

	I::GameMovement->StartTrackPredictionErrors(pLocal);

	Simulate(pLocal, pWeapon, pCmd);
}

void CEnginePrediction::Simulate(CTFPlayer* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd)
{
	if (!pLocal || !pLocal->IsAlive() || !pWeapon || !I::MoveHelper)
		return;

	nOldTickBase = pLocal->m_nTickBase();
	const bool bOldIsFirstPrediction = I::Prediction->m_bFirstTimePredicted;
	const bool bOldInPrediction = I::Prediction->m_bInPrediction;

	I::Prediction->m_bFirstTimePredicted = false;
	I::Prediction->m_bInPrediction = true;

	if(pCmd->weaponselect != NULL)
	{
		pLocal->SelectItem(pWeapon->GetName(), pCmd->weaponsubtype);
	}

	I::Prediction->SetLocalViewAngles(pCmd->viewangles);

	pLocal->PreThink();
	pLocal->Think();

	I::Prediction->SetupMove(pLocal, pCmd, I::MoveHelper, &m_MoveData);
	I::GameMovement->ProcessMovement(pLocal, &m_MoveData);
	I::Prediction->FinishMove(pLocal, pCmd, &m_MoveData);

	pLocal->PostThink();

	I::GameMovement->FinishTrackPredictionErrors(pLocal);

	I::Prediction->m_bFirstTimePredicted = bOldIsFirstPrediction;
	I::Prediction->m_bInPrediction = bOldInPrediction;
}

void CEnginePrediction::End(CTFPlayer* pLocal, CUserCmd* pCmd)
{
	if (!pLocal || !pLocal->IsAlive() || !I::MoveHelper)
		return;

	pLocal->m_nTickBase() = nOldTickBase;
	
	I::GlobalVars->tickcount = m_nOldTickCount;
	I::GlobalVars->curtime = m_fOldCurrentTime;
	I::GlobalVars->frametime = m_fOldFrameTime;

	pLocal->SetCurrentCmd(nullptr);

	*G::RandomSeed() = -1;
}
