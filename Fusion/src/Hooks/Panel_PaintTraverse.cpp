// this crashes, why?
#include "../SDK/SDK.h"

MAKE_HOOK(Panel_PaintTraverse, U::Memory.GetVFunc(I::VGuiPanel, 41), void, __fastcall,
	void* ecx, unsigned int VGuiPanel, bool forceRepaint, bool allowForce)
{
	/*const auto PanelName			= I::VGuiPanel->GetName(VGuiPanel);

	constexpr auto SteamFriendsList = FNV1A::HashConst("SteamFriendsList");
	constexpr auto RankPanel		= FNV1A::HashConst("RankPanel");
	constexpr auto avatar			= FNV1A::HashConst("avatar");*/

	if (I::VGuiPanel->GetName(VGuiPanel) == "SteamFriendsList" || I::VGuiPanel->GetName(VGuiPanel) == "RankPanel" || I::VGuiPanel->GetName(VGuiPanel) == "avatar")
		return;

	SDK::Output("PaintTraverse", I::VGuiPanel->GetName(VGuiPanel));
	CALL_ORIGINAL(ecx, VGuiPanel, forceRepaint, allowForce);
}