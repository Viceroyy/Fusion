#include "../SDK/SDK.h"

#include "../Features/Backtrack/Backtrack.h"
#include "../Features/CheaterDetection/CheaterDetection.h"
#include "../Features/CritHack/CritHack.h"
#include "../Features/Misc/Misc.h"
#include "../Features/Players/PlayerUtils.h"
#include "../Features/Resolver/Resolver.h"
#include "../Features/Simulation/MovementSimulation/MovementSimulation.h"
#include "../Features/Visuals/Visuals.h"

MAKE_SIGNATURE(CWeaponMedigun__UpdateEffects, "client.dll", "40 57 48 81 EC ? ? ? ? 8B 91 ? ? ? ? 48 8B F9 85 D2 0F 84 ? ? ? ? 48 89 B4 24", 0x0)
MAKE_SIGNATURE(CWeaponMedigun__StopChargeEffect, "client.dll", "40 53 48 83 EC ? 44 0F B6 C2", 0x0)
MAKE_SIGNATURE(CWeaponMedigun__ManageChargeEffect, "client.dll", "48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B F1 E8 ? ? ? ? 48 8B D8", 0x0)

MAKE_HOOK(BaseClientDLL_FrameStageNotify, U::Memory.GetVFunc(I::BaseClientDLL, 35), void, __fastcall,
	void* rcx, ClientFrameStage_t curStage)
{
	switch (curStage)
	{
	case FRAME_RENDER_START:
	{
		G::PunchAngles = Vec3();
		if (auto pLocal = H::Entities.GetLocal())
		{
			G::PunchAngles = pLocal->m_vecPunchAngle(); // use in aimbot
			if (Vars::Visuals::Removals::ViewPunch.Value)
				pLocal->m_vecPunchAngle() = Vec3(); // visual no-recoil
			F::Resolver.FrameStageNotify(pLocal);
			// CRASH: Exception thrown at 0x00007FFCD004E223 (client.dll) in tf_win64.exe: 0xC0000005: Access violation reading location 0x0000025800000000.
			// CRASH: Exception thrown at 0x00007FFC5A09EED0 (client.dll) in tf_win64.exe: 0xC0000005: Access violation reading location 0x000001F636472562.
			// crashes likely not fsn related
		}
		break;
	}
	}
	
	CALL_ORIGINAL(rcx, curStage);

	switch (curStage)
	{
	case FRAME_NET_UPDATE_START:
	{
		H::Entities.Clear();
		break;
	}
	case FRAME_NET_UPDATE_END:
	{
		H::Entities.Fill();
		for (auto& pEntity : H::Entities.GetGroup(EGroupType::PLAYERS_ALL))
		{
			if (pEntity->entindex() == I::EngineClient->GetLocalPlayer())
				continue; // local player managed in CPrediction_RunCommand

			G::VelocityMap[pEntity->entindex()] = { pEntity->m_vecOrigin(), pEntity->m_vecMaxs().z - pEntity->m_vecMins().z, pEntity->m_flSimulationTime() };

			static auto sv_maxusrcmdprocessticks = U::ConVars.FindVar("sv_maxusrcmdprocessticks");
			const int iTicks = sv_maxusrcmdprocessticks ? sv_maxusrcmdprocessticks->GetInt() : 24;
			if (auto iDifference = std::min(TIME_TO_TICKS(pEntity->m_flSimulationTime() - pEntity->m_flOldSimulationTime()), iTicks))
			{
				float flOldFrameTime = I::GlobalVars->frametime;
				I::GlobalVars->frametime = I::Prediction->m_bEnginePaused ? 0.f : TICK_INTERVAL;
				for (int i = 0; i < iDifference; i++)
				{
					G::UpdatingAnims = true;
					pEntity->As<CTFPlayer>()->UpdateClientSideAnimation();
					G::UpdatingAnims = false;
				}
				I::GlobalVars->frametime = flOldFrameTime;
			}
		}

		F::PlayerUtils.UpdatePlayers();
		F::Misc.DetectChoke();

		F::Backtrack.FrameStageNotify();
		F::MoveSim.FillVelocities();
		F::CritHack.Store();
		F::Visuals.StoreSightlines();

		F::CheaterDetection.Fill(); // maybe switch this to run instead

		break;
	}
	case FRAME_RENDER_START:
	{
		if (G::Unload)
			break;
		F::Visuals.SkyboxChanger();
		F::Visuals.Modulate();

		// Credits to Mad?, idek if this is the right frame for this, it works though
		static int OldMedigunEffect = 0;
		static int OldMedigunChargeEffect = 0;
		if (OldMedigunEffect != Vars::Visuals::Particle::MedigunBeamEffect.Value || OldMedigunChargeEffect != Vars::Visuals::Particle::MedigunChargeEffect.Value)
		{
			if (H::Entities.GetLocal() && H::Entities.GetWeapon() && H::Entities.GetWeapon()->m_iWeaponID() == TF_WEAPON_MEDIGUN)
			{
				// update all effects
				reinterpret_cast<void(__fastcall*)(void*)>(S::CWeaponMedigun__UpdateEffects())(H::Entities.GetWeapon());

				reinterpret_cast<void(__fastcall*)(void*)>(S::CWeaponMedigun__StopChargeEffect())(H::Entities.GetWeapon());
				reinterpret_cast<void(__fastcall*)(void*, bool)>(S::CWeaponMedigun__ManageChargeEffect())(H::Entities.GetWeapon(), false);
			}

			OldMedigunEffect = Vars::Visuals::Particle::MedigunBeamEffect.Value;
			OldMedigunChargeEffect = Vars::Visuals::Particle::MedigunChargeEffect.Value;
		}
		break;
	}
	}
}