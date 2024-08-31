#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_PlayerResource_GetPlayerName, "client.dll", "48 89 5C 24 ? 56 48 83 EC ? 48 63 F2", 0x0);

MAKE_HOOK(C_PlayerResource_GetPlayerName, S::C_PlayerResource_GetPlayerName(), const char*, __fastcall,
	void* ecx, int iIndex)
{
	if (!Vars::Visuals::UI::StreamerMode.Value)
		return CALL_ORIGINAL(ecx, iIndex);

	CTFPlayer* pEntity = I::ClientEntityList->GetClientEntity(iIndex)->As<CTFPlayer>();
	auto pLocal = H::Entities.GetLocal();
	if (!pEntity || !pLocal)
		return CALL_ORIGINAL(ecx, iIndex);

	if (iIndex == I::EngineClient->GetLocalPlayer())
		return "You";
	else if (H::Entities.IsFriend(iIndex))
		return "Friend";
	else if (pEntity->m_iTeamNum() != pLocal->m_iTeamNum())
		return "Enemy";
	else if (pEntity->m_iTeamNum() == pLocal->m_iTeamNum())
		return "Teammate";

	return CALL_ORIGINAL(ecx, iIndex);
}