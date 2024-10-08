#include "EnginePrediction.h"

#include "../TickHandler/TickHandler.h"

int CEnginePrediction::GetTickbase(CTFPlayer* pLocal, CUserCmd* pCmd)
{
	static int s_nTick = 0;
	static CUserCmd* s_pLastCmd = nullptr;

	if (pCmd)
	{
		if (!s_pLastCmd || s_pLastCmd->hasbeenpredicted)
			s_nTick = pLocal->m_nTickBase();
		else
			s_nTick++;

		s_pLastCmd = pCmd;
	}

	return s_nTick;
}

void CEnginePrediction::Start(CTFPlayer* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd)
{
	if (!pLocal || !pLocal->IsAlive() || !pWeapon || !pCmd || !I::MoveHelper)
		return;

	m_fOldCurrentTime = I::GlobalVars->curtime;
	m_fOldFrameTime = I::GlobalVars->frametime;
	m_nOldTickCount = I::GlobalVars->tickcount;

	nServerTicks = GetTickbase(pLocal, pCmd);

	I::GlobalVars->curtime = TICKS_TO_TIME(nServerTicks);
	I::GlobalVars->frametime = (I::Prediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL);
	I::GlobalVars->tickcount = nServerTicks;

	bSimulated = false;
	if (F::Ticks.GetTicks(pLocal) && Vars::CL_Move::Doubletap::AntiWarp.Value && pLocal->OnSolid())
		return; // hopefully more accurate eyepos while dting

	bSimulated = true;
	Simulate(pLocal, pWeapon, pCmd);
}

void CEnginePrediction::Simulate(CTFPlayer* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd)
{
	if(!pLocal || !pLocal->IsAlive() || !pWeapon || !pCmd || !I::MoveHelper)
		return;

	memset(&m_MoveData, 0, sizeof(CMoveData));

	pCmd->random_seed = MD5_PseudoRandom(pCmd->command_number) & std::numeric_limits<int>::max(); // I kinda remember cmd seed not being available here, maybe use *G::RandomSeed() again

	pLocal->m_pCurrentCommand() = pCmd;
	CBaseEntity::SetPredictionRandomSeed(pCmd);
	CBaseEntity::SetPredictionPlayer(pLocal);

	const int nOldTickBase = pLocal->m_nTickBase();
	const bool bOldIsFirstPrediction = I::Prediction->m_bFirstTimePredicted;
	const bool bOldInPrediction = I::Prediction->m_bInPrediction;

	I::Prediction->m_bFirstTimePredicted = false;
	I::Prediction->m_bInPrediction = true;

	I::GameMovement->StartTrackPredictionErrors(pLocal);

	if (pCmd->weaponselect != NULL)
	{
		CBaseCombatWeapon* pWeapon = dynamic_cast<CBaseCombatWeapon*>(I::ClientEntityList->GetClientEntity(pCmd->weaponselect));

		if (pWeapon)
			pLocal->SelectItem(pWeapon->GetName(), pCmd->weaponsubtype);
	}

	if (pCmd->impulse)
	{
		if (!pLocal->GetClientVehicle() || pLocal->UsingStandardWeaponsInVehicle()) // yeah this game has vehicles wdym
			pLocal->m_nImpulse() = pCmd->impulse;
	}

	pLocal->UpdateButtonState(pCmd->buttons);

	I::Prediction->SetLocalViewAngles(pCmd->viewangles);

	if (pLocal->PhysicsRunThink())
		pLocal->PreThink();

	const int thinktick = pLocal->m_nNextThinkTick();

	if (thinktick > 0 && thinktick < nServerTicks)
	{
		pLocal->m_nNextThinkTick() = -1; // TICK_NEVER_THINK
		pLocal->Think();
	}

	I::Prediction->SetupMove(pLocal, pCmd, I::MoveHelper, &m_MoveData);
	I::GameMovement->ProcessMovement(pLocal, &m_MoveData);
	I::Prediction->FinishMove(pLocal, pCmd, &m_MoveData);

	//pLocal->PostThink();

	pLocal->m_nTickBase() = nOldTickBase;

	I::Prediction->m_bInPrediction = bOldInPrediction;
	I::Prediction->m_bFirstTimePredicted = bOldIsFirstPrediction;
}

void CEnginePrediction::End(CTFPlayer* pLocal, CUserCmd* pCmd)
{
	if (!pLocal || !pLocal->IsAlive() || !I::MoveHelper)
		return;

	I::GameMovement->FinishTrackPredictionErrors(pLocal);

	pLocal->m_pCurrentCommand() = NULL;
	CBaseEntity::SetPredictionRandomSeed(NULL);
	CBaseEntity::SetPredictionPlayer(NULL);

	I::GlobalVars->curtime = m_fOldCurrentTime;
	I::GlobalVars->frametime = m_fOldFrameTime;
	I::GlobalVars->tickcount = m_nOldTickCount;
}
