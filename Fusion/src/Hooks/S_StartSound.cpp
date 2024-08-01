#include "../SDK/SDK.h"

#include "../Features/Visuals/Visuals.h"

MAKE_SIGNATURE(S_StartSound, "engine.dll", "40 53 48 83 EC ? 48 83 79 ? ? 48 8B D9 75 ? 33 C0", 0x0);

const static std::vector<std::string> NOISEMAKER_SOUNDS{ "items/halloween", "items/football_manager", "items/japan_fundraiser", "items/samurai", "items/summer", "misc/happy_birthday_tf", "misc/jingle_bells" };

MAKE_HOOK(S_StartSound, S::S_StartSound(), int, __cdecl,
	StartSoundParams_t& params)
{
	// Noisemaker
	if (Vars::Misc::Sound::Block.Value & (1 << 1))
	{
		for (auto& sound : NOISEMAKER_SOUNDS)
		{
			if (params.pSfx->.find(sound) != std::string::npos)
				return;
		}
	}
	return CALL_ORIGINAL(params);
}