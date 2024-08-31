#include "../SDK/SDK.h"

MAKE_SIGNATURE(CTFWearable_ShouldDraw, "client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC ? 8B 91", 0x0);
MAKE_SIGNATURE(CTFWearable_InternalDrawModel, "client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 44 8B 81", 0x0);

MAKE_HOOK(CTFWearable_ShouldDraw, S::CTFWearable_ShouldDraw(),
	bool, __fastcall, void* rcx)
{
	if (Vars::Visuals::Removals::Cosmetics.Value)
		return false;

	return CALL_ORIGINAL(rcx);
}

MAKE_HOOK(CTFWearable_InternalDrawModel, S::CTFWearable_InternalDrawModel(),
	int, __fastcall, void* rcx, int flags)
{
	if (Vars::Visuals::Removals::Cosmetics.Value)
		return 0;

	return CALL_ORIGINAL(rcx, flags);
}