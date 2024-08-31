#include "../SDK/SDK.h"

MAKE_HOOK(StudioRender_SetColorModulation, U::Memory.GetVFunc(I::StudioRender, 27), void, __fastcall,
	void* rcx, const float* pColor)
{
	if (Vars::Visuals::World::Modulations.Value & (1 << 2) && G::DrawingProps && !(Vars::Visuals::UI::CleanScreenshots.Value && I::EngineClient->IsTakingScreenshot()))
	{
		// TODO: make these static?
		auto Red = Vars::Colors::PropModulation.Value.r;
		auto Green = Vars::Colors::PropModulation.Value.g;
		auto Blue = Vars::Colors::PropModulation.Value.b;

		// Assert the values to make sure they aren't 0, we're dividing here.
		if (Red || Green || Blue)
		{
			float flRed = Red / 255.f;
			float flGreen = Green / 255.f;
			float flBlue = Blue / 255.f;

			const float flBlend[3] = {
				flRed,
				flGreen,
				flBlue
			};

			return CALL_ORIGINAL(rcx, flBlend);
		}
	}

	CALL_ORIGINAL(rcx, pColor);
}