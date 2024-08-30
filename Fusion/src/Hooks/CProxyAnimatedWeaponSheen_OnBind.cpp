#include "../SDK/SDK.h"

// Credits to M-Fed

struct CProxyAnimatedWeaponSheen
{
	__int64 balls;
	IMaterialVar* m_AnimatedTextureVar;
	IMaterialVar* m_AnimatedTextureFrameNumVar;
	float m_FrameRate;
	__declspec(align(4)) bool m_WrapAnimation;
	IMaterialVar* m_pSheenIndexVar;
	IMaterialVar* m_pTintVar;
	IMaterialVar* m_pSheenVar;
	IMaterialVar* m_pSheenMaskVar;
	IMaterialVar* m_pScaleXVar;
	IMaterialVar* m_pScaleYVar;
	IMaterialVar* m_pOffsetXVar;
	IMaterialVar* m_pOffsetYVar;
	IMaterialVar* m_pDirectionVar;
	float m_flNextStartTime;
	float m_flScaleX;
	float m_flScaleY;
	float m_flSheenOffsetX;
	float m_flSheenOffsetY;
	int m_iSheenDir;
};

MAKE_SIGNATURE(CProxyAnimatedWeaponSheen_OnBind, "client.dll", "48 89 54 24 ? 55 57 41 54 48 8D 6C 24", 0x0);

MAKE_HOOK(AnimatedWeaponSheen_OnBind, S::CProxyAnimatedWeaponSheen_OnBind(), void, __fastcall, 
	CProxyAnimatedWeaponSheen* rcx, void* pEntity)
{
	CALL_ORIGINAL(rcx, pEntity);

	if (!Vars::Visuals::Particle::RainbowKillstreakSheen.Value || !rcx)
		return;

	IMaterialVar* m_pTintVar = rcx->m_pTintVar;
	if (!m_pTintVar)
		return;

	const float* color = m_pTintVar->GetVecValueInternal();
	if (!color || color[3] == 0.f)
		return;

	float r = (static_cast<float>(floor(sin(I::GlobalVars->curtime + 0) * 128.0f + 128.0f))) / 255.f;
	float g = (static_cast<float>(floor(sin(I::GlobalVars->curtime + 2) * 128.0f + 128.0f))) / 255.f;
	float b = (static_cast<float>(floor(sin(I::GlobalVars->curtime + 4) * 128.0f + 128.0f))) / 255.f;
	float colorArray[4] = { r, g, b, 1.f };

	m_pTintVar->SetVecValue(colorArray, 4);
}