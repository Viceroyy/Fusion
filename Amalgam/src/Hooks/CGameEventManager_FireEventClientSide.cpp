//#include "../SDK/SDK.h"
//#include "../Features/Killstreak/Killstreak.h"
//
//MAKE_HOOK(CGameEventManager_FireEventClientSide, U::Memory.GetVFunc(I::GameEventManager, 8), bool, __fastcall,
//	void* ecx, void* edx, IGameEvent* pEvent)
//{
//	if (!ecx || !edx || !pEvent) { return false; }
//	const FNV1A_t uNameHash = FNV1A::Hash(pEvent->GetName());
//	F::Killstreaker.FireEvents(pEvent, uNameHash);
//
//	return CALL_ORIGINAL(ecx, edx, pEvent);
//}