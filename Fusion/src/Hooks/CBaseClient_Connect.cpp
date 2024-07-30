#include "../SDK/SDK.h"

#include "../Features/Visuals/Materials/Materials.h"

MAKE_SIGNATURE(CBaseClient_Connect, "engine.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B D9 49 8B E9 48 8D 0D", 0x0);

MAKE_HOOK(CBaseClient_Connect, S::CBaseClient_Connect(), void, __fastcall,
	void* ecx, const char* szName, int nUserID, INetChannel* pNetChannel, bool bFakePlayer, int clientChallenge)
{
	F::Materials.ReloadMaterials();

	/*if (Vars::Visuals::Other::ThePS2Inator.Value)
	{
		static auto r_lod = I::CVar->FindVar("r_lod");
		static auto r_rootlod = I::CVar->FindVar("r_rootlod");
		static auto mat_picmip = I::CVar->FindVar("mat_picmip");

		if (r_lod)
			r_lod->SetValue(7);
		if (r_rootlod)
			r_rootlod->SetValue(7);
		if (mat_picmip)
			mat_picmip->SetValue(4);
	}*/

	CALL_ORIGINAL(ecx, szName, nUserID, pNetChannel, bFakePlayer, clientChallenge);
}