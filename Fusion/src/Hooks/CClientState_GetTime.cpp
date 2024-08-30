#include "../SDK/SDK.h"

MAKE_SIGNATURE(CClientState_GetTime, "engine.dll", "80 B9 ? ? ? ? ? 66 0F 6E 81", 0x0);
MAKE_SIGNATURE(CL_FireEvents_CClientState_GetTime_Call, "engine.dll", "E8 ? ? ? ? 0F 2F 45", 0x5);

MAKE_HOOK(CClientState_GetTime, S::CClientState_GetTime(), float, __fastcall,
	void* rcx)
{
	static auto dwDesired = S::CL_FireEvents_CClientState_GetTime_Call();
	const auto dwRetAddr = std::uintptr_t(_ReturnAddress());

	if (dwRetAddr == dwDesired)
		return std::numeric_limits<float>::max();

	return CALL_ORIGINAL(rcx);
}
