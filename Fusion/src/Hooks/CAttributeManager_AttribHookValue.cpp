#include "../SDK/SDK.h"

// Credits to Mad?

MAKE_SIGNATURE(CAttributeManager_AttribHookValue, "client.dll", "4C 8B DC 49 89 5B ? 49 89 6B ? 49 89 73 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 48 8B 3D ? ? ? ? 4C 8D 35", 0x0);
MAKE_SIGNATURE(FireEvent_AttribHookValue_Call, "client.dll", "E8 ? ? ? ? 8B F8 83 BE", 0x5);

MAKE_HOOK(CAttributeManager_AttribHookValue, S::CAttributeManager_AttribHookValue(), int, __fastcall,
	int baseVal, const char* str, void* pEnt, void* buffer, bool isGlobalConstStr)
{
	if (!Vars::Visuals::Particle::RainbowFootstepEffect.Value || I::EngineClient->IsTakingScreenshot() && Vars::Visuals::UI::CleanScreenshots.Value)
		return CALL_ORIGINAL(baseVal, str, pEnt, buffer, isGlobalConstStr);

	static const auto dwDesired = S::FireEvent_AttribHookValue_Call();
	const auto dwRetAddr = std::uintptr_t(_ReturnAddress());

	if (dwRetAddr == dwDesired)
	{
		static constexpr int HashedFootstepType = FNV1A::HashConst("halloween_footstep_type");
		auto Hashed = FNV1A::Hash(str);
		if (Hashed == HashedFootstepType)
		{
			Color_t Color = SDK::Rainbow();
			return std::stoul(std::format("0x{:02X}{:02X}{:02X}", Color.r, Color.g, Color.b), nullptr, 16);
		}
	}

	return CALL_ORIGINAL(baseVal, str, pEnt, buffer, isGlobalConstStr);
}