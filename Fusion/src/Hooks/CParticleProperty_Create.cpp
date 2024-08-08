#include "../SDK/SDK.h"

// Credits to Mad? for the idea and medigun particle changer
// Credits to yaya for the x86 sigs, rocket trail particle changer and the whole code :trollface:

MAKE_SIGNATURE(CParticleProperty_Create, "client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B 59 ? 49 8B F1", 0x0);

MAKE_SIGNATURE(UpdateEffectsCall1, "client.dll", "E8 ? ? ? ? 49 8B CC F3 0F 11 74 24", 0x5);
MAKE_SIGNATURE(UpdateEffectsCall2, "client.dll", "E8 ? ? ? ? 41 8B 14 24 48 8B D8", 0x5);
MAKE_SIGNATURE(ManageChargeEffectCall, "client.dll", "E8 ? ? ? ? 48 89 86 ? ? ? ? 48 89 BE", 0x5);
MAKE_SIGNATURE(Rockettrail_UnderwaterCall, "client.dll", "E8 ? ? ? ? 48 8B 5C 24 ? 40 38 B7", 0x5);

MAKE_HOOK(CParticleProperty_Create, S::CParticleProperty_Create(), int, __fastcall,
	void* ecx, const char* pszParticleName, ParticleAttachment_t iAttachType, const char* pszAttachmentName)
{
	if (I::EngineClient->IsTakingScreenshot() && Vars::Visuals::UI::CleanScreenshots.Value)
        return CALL_ORIGINAL(ecx, pszParticleName, iAttachType, pszAttachmentName);

    if (auto pLocal = H::Entities.GetLocal())
    {
        static auto UpdateEffectsCall1 = S::UpdateEffectsCall1();
        static auto UpdateEffectsCall2 = S::UpdateEffectsCall2();
        static auto ManageChargeEffectCall = S::ManageChargeEffectCall();
        static auto Rockettrail_UnderwaterCall = S::Rockettrail_UnderwaterCall();
        
        const auto dwRetAddr = std::uintptr_t(_ReturnAddress());

        if (dwRetAddr == UpdateEffectsCall1 || dwRetAddr == UpdateEffectsCall2)
        {
            switch (Vars::Visuals::Particle::MedigunBeamEffect.Value)
            {
                case 0: break;
                case 1: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "dispenser_heal_blue" : "dispenser_heal_red"; break;
                case 2: pszParticleName = "passtime_beam"; break;
                case 3: pszParticleName = "bombonomicon_spell_trail"; break;
                case 4: pszParticleName = "medicgun_beam_machinery_stage3"; break;
                case 5: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "medicgun_beam_blue_invun" : "medicgun_beam_red_invun"; break;
            }
        }

        if (dwRetAddr == ManageChargeEffectCall)
        {
            switch (Vars::Visuals::Particle::MedigunChargeEffect.Value)
            {
                case 0: break;
                case 1: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "spell_fireball_small_trail_blue" : "spell_fireball_small_trail_red"; break;
                case 2: pszParticleName = "unusual_spellbook_circle_purple"; break;
                case 3: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "electrocuted_blue" : "electrocuted_red"; break;
                case 4: pszParticleName = "unusual_hearts_bubbling"; break;
                case 5: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "unusual_robot_orbiting_sparks" : "unusual_robot_orbiting_sparks2"; break;
                case 6: pszParticleName = "unusual_storm_knives_clouds"; break;
                case 7: pszParticleName = "unusual_eotl_frostbite"; break;
                case 8: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "unusual_souls_green_parent" : "unusual_souls_purple_parent"; break;
                case 9: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "unusual_robot_time_warp" : "unusual_robot_time_warp2"; break;
                case 10: pszParticleName = "unusual_storm_spooky"; break;
                case 11: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "unusual_zap_green" : "unusual_zap_yellow"; break;
                case 12: pszParticleName = "superrare_beams1"; break;
                case 13: pszParticleName = "superrare_beams1_newstyle"; break;
                case 14: pszParticleName = "utaunt_disco_beams3"; break;
                case 15: pszParticleName = "ghost_pumpkin"; break;
            }
        }

        if (dwRetAddr == Rockettrail_UnderwaterCall)
        {
            switch (Vars::Visuals::Particle::RocketTrailEffect.Value)
            {
                case 0: break;
                case 1: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "critical_rocket_blue" : "critical_rocket_red"; break;
                case 2: pszParticleName = "rockettrail_underwater"; break;
                case 3: pszParticleName = "halloween_rockettrail"; break;
                case 4: pszParticleName = "rockettrail_airstrike_line"; break;
                case 5: pszParticleName = "eyeboss_projectile"; break;
                case 6: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "drg_cow_rockettrail_normal_blue" : "drg_cow_rockettrail_normal"; break;
                case 7: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "drg_cow_rockettrail_charged_blue" : "drg_cow_rockettrail_charged"; break;
            }
        }
    }

    return CALL_ORIGINAL(ecx, pszParticleName, iAttachType, pszAttachmentName);
}

MAKE_SIGNATURE(CParticleProperty_Create2, "client.dll", "44 89 4C 24 ? 44 89 44 24 ? 53", 0x0);

MAKE_SIGNATURE(Rockettrail_AirstrikeCall, "client.dll", "E8 ? ? ? ? 48 8B CF E8 ? ? ? ? 48 8B D8 48 85 C0 0F 84", 0x5);
MAKE_SIGNATURE(Rockettrail_bUsingCustomCall, "client.dll", "E8 ? ? ? ? EB ? 4C 8D 0D ? ? ? ? 41 B8 ? ? ? ? 48 8D 15", 0x5);
MAKE_SIGNATURE(Halloween_RockettrailCall, "client.dll", "E8 ? ? ? ? E9 ? ? ? ? 48 85 DB 0F 85", 0x5);
MAKE_SIGNATURE(CTFProjectile_EnergyBallCall, "client.dll", "E8 ? ? ? ? 48 8B 7C 24 ? 48 89 83", 0x5);

MAKE_HOOK(CParticleProperty_Create2, S::CParticleProperty_Create2(), int, __fastcall,
    void* ecx, const char* pszParticleName, ParticleAttachment_t iAttachType, int iAttachmentPoint, Vector vecOriginOffset)
{
    if (I::EngineClient->IsTakingScreenshot() && Vars::Visuals::UI::CleanScreenshots.Value)
        return CALL_ORIGINAL(ecx, pszParticleName, iAttachType, iAttachmentPoint, vecOriginOffset);

    if (auto pLocal = H::Entities.GetLocal())
    {
        static auto Rockettrail_AirstrikeCall = S::Rockettrail_AirstrikeCall();
        static auto Rockettrail_bUsingCustomCall = S::Rockettrail_bUsingCustomCall();
        static auto Halloween_RockettrailCall = S::Halloween_RockettrailCall();
        static auto CTFProjectile_EnergyBallCall = S::CTFProjectile_EnergyBallCall();

        const auto dwRetAddr = std::uintptr_t(_ReturnAddress());

        if (dwRetAddr == Rockettrail_AirstrikeCall || dwRetAddr == Rockettrail_bUsingCustomCall || dwRetAddr == Halloween_RockettrailCall || dwRetAddr == CTFProjectile_EnergyBallCall)
        {
            switch (Vars::Visuals::Particle::RocketTrailEffect.Value)
            {
                case 0: break;
                case 1: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "critical_rocket_blue" : "critical_rocket_red"; break;
                case 2: pszParticleName = "rockettrail_underwater"; break;
                case 3: pszParticleName = "halloween_rockettrail"; break;
                case 4: pszParticleName = "rockettrail_airstrike_line"; break;
                case 5: pszParticleName = "eyeboss_projectile"; break;
                case 6: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "drg_cow_rockettrail_normal_blue" : "drg_cow_rockettrail_normal"; break;
                case 7: pszParticleName = pLocal->m_iTeamNum() == TF_TEAM_BLUE ? "drg_cow_rockettrail_charged_blue" : "drg_cow_rockettrail_charged"; break;
            }
        }
    }

    return CALL_ORIGINAL(ecx, pszParticleName, iAttachType, iAttachmentPoint, vecOriginOffset);
}