// We use S_StartDynamicSound for the far esp, I did this because I was bored, also the origin pos seems to be wrong here
#include "../SDK/SDK.h"

struct SoundInfo_t
{
	int				nSequenceNumber;
	int				nEntityIndex;
	int				nChannel;
	const char*		pszName;		// UNDONE: Make this a FilenameHandle_t to avoid bugs with arrays of these
	Vector			vOrigin;
	Vector			vDirection;
	float			fVolume;
	soundlevel_t	Soundlevel;
	bool			bLooping;
	int				nPitch;
	int				nSpecialDSP;
	Vector			vListenerOrigin;
	int				nFlags;
	int 			nSoundNum;
	float			fDelay;
	bool			bIsSentence;
	bool			bIsAmbient;
	int				nSpeakerEntity;
};

MAKE_SIGNATURE(CL_DispatchSound, "engine.dll", "48 8B C4 48 89 78 ? 55 48 8D 68", 0x0);

MAKE_HOOK(CL_DispatchSound, S::CL_DispatchSound(), int, __fastcall,
	const SoundInfo_t& sound)
{
	if (sound.nEntityIndex <= 0)
		return CALL_ORIGINAL(sound);

	Vector vOrigin = sound.vOrigin;
	const int iEntIndex = sound.nEntityIndex;
	auto pEntity = I::ClientEntityList->GetClientEntity(iEntIndex);

	if (pEntity && iEntIndex != I::EngineClient->GetLocalPlayer() && pEntity->IsDormant() && pEntity->GetClassID() == ETFClassID::CTFPlayer)
		G::DormancyMap[iEntIndex] = { vOrigin, I::EngineClient->Time() };

	return CALL_ORIGINAL(sound);
}