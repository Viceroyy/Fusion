#include "../SDK/SDK.h"

#include "../Features/Visuals/Materials/Materials.h"

MAKE_HOOK(ViewRender_LevelShutdown, U::Memory.GetVFunc(I::ViewRender, 2), void, __fastcall,
	void* rcx)
{
	F::Materials.UnloadMaterials();

	CALL_ORIGINAL(rcx);
}