#include "../SDK/SDK.h"

#include "../Features/Visuals/Materials/Materials.h"
#include "../Features/Visuals/Visuals.h"
#include "../Features/Backtrack/Backtrack.h"
#include "../Features/CheaterDetection/CheaterDetection.h"
#include "../Features/NoSpread/NoSpreadHitscan/NoSpreadHitscan.h"
#include "../Features/TickHandler/TickHandler.h"

MAKE_HOOK(ViewRender_LevelInit, U::Memory.GetVFunc(I::ViewRender, 1), void, __fastcall,
	void* ecx)
{
	F::Materials.ReloadMaterials();
	F::Visuals.OverrideWorldTextures();

	F::Backtrack.Restart();
	F::Ticks.Reset();
	F::NoSpreadHitscan.Reset(true);
	F::CheaterDetection.Reset();

	if (Vars::Visuals::Other::ThePS2Inator.Value)
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
	}
	CALL_ORIGINAL(ecx);
}
