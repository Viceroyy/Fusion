#include "SpectatorList.h"

bool CSpectatorList::GetSpectators(CTFPlayer* pLocal)
{
	Spectators.clear();

	for (auto pEntity : H::Entities.GetGroup(EGroupType::PLAYERS_ALL))
	{
		auto pPlayer = pEntity->As<CTFPlayer>();

		if (!pPlayer->IsAlive() && pPlayer->m_hObserverTarget().Get() == pLocal)
		{
			std::wstring szMode;
			switch (pPlayer->m_iObserverMode())
			{
				case OBS_MODE_DEATHCAM:		{ szMode = L"Deathcam"; break; }
				case OBS_MODE_FREEZECAM:	{ szMode = L"Freezecam"; break; }
				case OBS_MODE_FIRSTPERSON:	{ szMode = L"1st"; break; }
				case OBS_MODE_THIRDPERSON:	{ szMode = L"3rd"; break; }
				default:					{ continue; }
			}

			int respawnIn = 0; float respawnTime = 0;
			if (auto pResource = H::Entities.GetPR())
			{
				respawnTime = pResource->GetNextRespawnTime(pPlayer->entindex());
				respawnIn = std::max(int(respawnTime - I::GlobalVars->curtime), 0);
			}
			bool respawnTimeIncreased = false; // theoretically the respawn times could be changed by the map but oh well
			if (!RespawnCache.contains(pPlayer->entindex()))
				RespawnCache[pPlayer->entindex()] = respawnTime;
			if (RespawnCache[pPlayer->entindex()] + 0.9f < respawnTime)
			{
				respawnTimeIncreased = true;
				RespawnCache[pPlayer->entindex()] = -1.f;
			}

			PlayerInfo_t pi{};
			if (I::EngineClient->GetPlayerInfo(pPlayer->entindex(), &pi))
			{
				std::wstring name = SDK::ConvertUtf8ToWide(pi.name);
				if (Vars::Visuals::UI::StreamerMode.Value)
				{
					if (H::Entities.IsFriend(pPlayer->entindex()))
						name = L"Friend";
					else if (pPlayer->m_iTeamNum() != pLocal->m_iTeamNum())
						name = L"Enemy";
					else if (pPlayer->m_iTeamNum() == pLocal->m_iTeamNum())
						name = L"Teammate";
				}

				Spectators.push_back({
					name, szMode, respawnIn, respawnTimeIncreased, H::Entities.IsFriend(pPlayer->entindex()),
					pPlayer->m_iTeamNum(), pPlayer->entindex()
				});
			}
		}
		else
		{
			auto iter = RespawnCache.find(pPlayer->entindex());
			if (iter != RespawnCache.end())
				RespawnCache.erase(iter);
		}
	}

	return !Spectators.empty();
}

void CSpectatorList::Run(CTFPlayer* pLocal)
{
	if (!(Vars::Menu::Indicators.Value & (1 << 2)))
	{
		RespawnCache.clear();
		return;
	}

	if (!pLocal->IsAlive() || !GetSpectators(pLocal))
		return;

	int x = Vars::Menu::SpectatorsDisplay.Value.x;
	int iconOffset = 0;
	int y = Vars::Menu::SpectatorsDisplay.Value.y + 8;

	EAlign align = ALIGN_TOP;
	if (x <= (100 + 50 * Vars::Menu::DPI.Value))
	{
		x -= 42 * Vars::Menu::DPI.Value;
		align = ALIGN_TOPLEFT;
	}
	else if (x >= H::Draw.m_nScreenW - (100 + 50 * Vars::Menu::DPI.Value))
	{
		x += 42 * Vars::Menu::DPI.Value;
		align = ALIGN_TOPRIGHT;
	}

	const auto& fFont = H::Fonts.GetFont(FONT_INDICATORS);

	H::Draw.String(fFont, x, y, Vars::Menu::Theme::Accent.Value, align, L"Spectating You:");
	for (const auto& Spectator : Spectators)
	{
		y += fFont.m_nTall + 3;

		Color_t color = Vars::Menu::Theme::Active.Value;
		if (Spectator.Mode == std::wstring{L"1st"})
			color = { 255, 200, 127, 255 };
		if (Spectator.RespawnTimeIncreased)
			color = SDK::WarningColor();
		if (Spectator.IsFriend)
			color = { 200, 255, 200, 255 };
		if (Spectator.Team == pLocal->m_iTeamNum())
			H::Draw.String(fFont, x + iconOffset, y, color, align, L"%ls - %ls (respawn %ds)", Spectator.Name.data(), Spectator.Mode.data(), Spectator.RespawnIn);
		else
			H::Draw.String(fFont, x + iconOffset, y, Vars::Menu::Theme::Accent.Value, align, L"%ls - %ls", Spectator.Name.data(), Spectator.Mode.data());
	}
}
