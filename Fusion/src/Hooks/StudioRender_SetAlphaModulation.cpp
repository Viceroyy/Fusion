#include "../SDK/SDK.h"

MAKE_HOOK(StudioRender_SetAlphaModulation, U::Memory.GetVFunc(I::StudioRender, 28), void, __fastcall,
	void* ecx, float flAlpha)
{
	if (Vars::Visuals::World::Modulations.Value & (1 << 2) &&
		G::DrawingProps && !(Vars::Visuals::UI::CleanScreenshots.Value && I::EngineClient->IsTakingScreenshot()))
	{
		// TODO: make these static?
		auto PropVal = Vars::Colors::PropModulation.Value.a;
		auto flHookAlpha = flAlpha;

		if (PropVal || flHookAlpha)
		{
			auto flVal = PropVal / 255.f * flHookAlpha;
			return CALL_ORIGINAL(ecx, flVal);
		}
	}
	CALL_ORIGINAL(ecx, flAlpha);
}