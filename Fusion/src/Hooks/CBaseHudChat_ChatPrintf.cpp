#include "../SDK/SDK.h"

#include "../Features/Players/PlayerUtils.h"

std::string ColorRed = { '\x7', 'F', 'F', '3', 'F', '3', 'F' };
std::string ColorBlue = { '\x7', '9', '9', 'C', 'C', 'F', 'F' };

MAKE_HOOK(CBaseHudChat_ChatPrintf, U::Memory.GetVFunc(I::ClientModeShared->m_pChatElement, 19), void, __fastcall,
	void* ecx, int iPlayerIndex, int iFilter, const char* fmt, ...)
{
	va_list marker;
	char buffer[4096];
	va_start(marker, fmt);
	vsnprintf_s(buffer, sizeof(buffer), fmt, marker);
	va_end(marker);

	if (strlen(buffer) > 0 && buffer[strlen(buffer) - 1] == '\n')
		buffer[strlen(buffer) - 1] = 0;
	char* msg = buffer;
	while (*msg && (*msg == '\n' || *msg == '\r' || *msg == '\x1A'))
		msg++;
	if (!*msg)
		return;

	std::string finalMsg = msg, name = {};

	PlayerInfo_t pi{};
	if (!I::EngineClient->GetPlayerInfo(iPlayerIndex, &pi))
		return CALL_ORIGINAL(ecx, iPlayerIndex, iFilter, "%s", finalMsg.c_str());

	name = pi.name;
	if (finalMsg.find(name) == std::string::npos)
		return CALL_ORIGINAL(ecx, iPlayerIndex, iFilter, "%s", finalMsg.c_str());

	if (iPlayerIndex && Vars::Misc::Chat::Tags.Value)
	{
		std::string tag = "", color = "";
		if (iPlayerIndex == I::EngineClient->GetLocalPlayer())
			tag = "You", color = Vars::Colors::Local.Value.ToHexA();
		else if (H::Entities.IsFriend(iPlayerIndex))
			tag = "Friend", color = F::PlayerUtils.mTags["Friend"].Color.ToHexA();
		else
		{
			std::string sTag; PriorityLabel_t plTag;
			if (F::PlayerUtils.GetSignificantTag(pi.friendsID, &sTag, &plTag, 0))
				tag = sTag, color = plTag.Color.ToHexA();
		}

		if (tag.length())
		{
			finalMsg = std::format("{}[{}] \x3{}", color, tag, finalMsg);
			if (auto offset = finalMsg.find(name))
				finalMsg = finalMsg.replace(offset + name.length(), 0, "\x1");
		}
	}

	CTFPlayer* pEntity = I::ClientEntityList->GetClientEntity(iPlayerIndex)->As<CTFPlayer>();
	auto pLocal = H::Entities.GetLocal();
	if (Vars::Visuals::UI::StreamerMode.Value && pEntity && pLocal)
	{
		std::string changedname;
		if (iPlayerIndex == I::EngineClient->GetLocalPlayer())
			changedname = "You";
		else if (H::Entities.IsFriend(iPlayerIndex))
			changedname = "Friend";
		else if (pEntity->m_iTeamNum() != pLocal->m_iTeamNum())
			changedname = "Enemy";
		else if (pEntity->m_iTeamNum() == pLocal->m_iTeamNum())
			changedname = "Teammate";

		std::string newname = pEntity->IsAlive() ? (changedname + "\x1") : changedname;
		std::string finalname = (pEntity->m_iTeamNum() == TF_TEAM_RED ? ColorRed : ColorBlue) + newname;
		std::string finalmessage = (pEntity->IsAlive() ? "" : "\x1*DEAD* ") + finalname;
		finalMsg = finalMsg.replace(0, finalMsg.find(name) + name.length(), finalmessage.c_str());
	}

	CALL_ORIGINAL(ecx, iPlayerIndex, iFilter, "%s", finalMsg.c_str());
}