//#include "Killstreak.h"
//
//void CKillstreaker::ResetKillstreak()
//{
//	Killstreak = 0;
//	KillstreakMap.clear();
//}
//
//int CKillstreaker::GetCurrentStreak()
//{
//	return Killstreak;
//}
//
//int CKillstreaker::GetCurrentWeaponStreak()
//{
//	return KillstreakMap[H::Entities.GetWeapon()->entindex()];
//}
//
//class NetworkVar_m_nStreaks
//{
//public:
//	const int operator[](int i) const
//	{
//		return Get(i);
//	}
//	const int& Get(int i) const
//	{
//		return m_Value[i];
//	}
//	int& GetForModify(int i)
//	{
//		return m_Value[i];
//	}
//	void Set(int i, const int& val)
//	{
//		if (memcmp(&m_Value[i], &val, sizeof(int)))
//		{
//			m_Value[i] = val;
//		}
//	}
//	const int* Base() const
//	{
//		return m_Value;
//	}
//	int Count() const
//	{
//		return kTFStreak_COUNT;
//	}
//	int m_Value[kTFStreak_COUNT];
//};
//
//void CKillstreaker::ApplyKillstreak()
//{
//	//if (Vars::Misc::KillstreakWeapon.Value)
//	{
//		if (const auto& pLocal = H::Entities.GetLocal())
//		{
//			if (!H::Entities.GetPR()) return;
//			const auto streaksResource = H::Entities.GetPR()->GetStreaks(I::EngineClient->GetLocalPlayer());
//			if (streaksResource && *streaksResource != GetCurrentStreak())
//			{
//				streaksResource[0] = GetCurrentStreak();
//				streaksResource[1] = GetCurrentStreak();
//				streaksResource[2] = GetCurrentStreak();
//				streaksResource[3] = GetCurrentStreak();
//			}
//
//			auto streaksPlayer = reinterpret_cast<NetworkVar_m_nStreaks*>(*((DWORD*)pLocal + 996)); //C_TFPlayer CTFPlayerShared::m_nStreaks
//			if (!streaksPlayer ||
//				(DWORD)streaksPlayer < 0x1000 ||
//				!streaksPlayer->m_Value ||
//				(DWORD)streaksPlayer->m_Value < 0x1000)
//			{
//				return;
//			}
//			if (streaksPlayer && streaksPlayer->m_Value)
//			{
//				streaksPlayer->Set(0, GetCurrentStreak());
//				streaksPlayer->Set(1, GetCurrentStreak());
//				streaksPlayer->Set(2, GetCurrentStreak());
//				streaksPlayer->Set(3, GetCurrentStreak());
//			}
//		}
//	}
//}
//
//void CKillstreaker::PlayerDeath(IGameEvent* pEvent)
//{
//	const int attacker = I::EngineClient->GetPlayerForUserID(pEvent->GetInt("attacker"));
//	const int userid = I::EngineClient->GetPlayerForUserID(pEvent->GetInt("userid"));
//
//	if (userid == I::EngineClient->GetLocalPlayer())
//	{
//		ResetKillstreak();
//		return;
//	}
//
//	if (attacker != I::EngineClient->GetLocalPlayer() ||
//		attacker == userid ||
//		!H::Entities.GetLocal() || !H::Entities.GetWeapon() || H::Entities.GetLocal()->deadflag())
//	{
//		return;
//	}
//
//	Killstreak++;
//	KillstreakMap[H::Entities.GetWeapon()->entindex()]++;
//
//	pEvent->SetInt("kill_streak_total", GetCurrentStreak());
//	pEvent->SetInt("kill_streak_wep", GetCurrentWeaponStreak());
//
//	ApplyKillstreak();
//}
//
//void CKillstreaker::PlayerSpawn(IGameEvent* pEvent)
//{
//	const int userid = I::EngineClient->GetPlayerForUserID(pEvent->GetInt("userid"));
//
//	if (userid == I::EngineClient->GetLocalPlayer())
//	{
//		ResetKillstreak();
//	}
//	ApplyKillstreak();
//}
//
//void CKillstreaker::FireEvents(IGameEvent* pEvent, const FNV1A_t uNameHash)
//{
//	//if (Vars::Misc::KillstreakWeapon.Value)
//	{
//		if (uNameHash == FNV1A::HashConst("player_death"))
//		{
//			PlayerDeath(pEvent);
//		}
//		else if (uNameHash == FNV1A::HashConst("player_spawn"))
//		{
//			PlayerSpawn(pEvent);
//		}
//	}
//}
