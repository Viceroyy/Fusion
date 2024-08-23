#include "../SDK/SDK.h"

MAKE_SIGNATURE(CTFRagdoll_CreateTFRagdoll, "client.dll", "48 89 4C 24 ? 55 53 56 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 8B 91", 0x0);

void ClearEffects(CTFRagdoll* pRagdoll)
{
	pRagdoll->m_bBurning() = false;
	pRagdoll->m_bElectrocuted() = false;
	pRagdoll->m_bBecomeAsh() = false;
	pRagdoll->m_bDissolving() = false;
	pRagdoll->m_bGoldRagdoll() = false;
	pRagdoll->m_bIceRagdoll() = false;
}

MAKE_HOOK(CTFRagdoll_CreateTFRagdoll, S::CTFRagdoll_CreateTFRagdoll(), void, __fastcall,
	void* ecx)
{
	if (Vars::Visuals::Ragdolls::NoRagdolls.Value)
		return;

	if (!Vars::Visuals::Ragdolls::Enabled.Value)
		return CALL_ORIGINAL(ecx);

	if (const auto& pRagdoll = reinterpret_cast< CTFRagdoll* >( ecx ))
	{
		if ( Vars::Visuals::Ragdolls::EnemyOnly.Value )
		{
			const auto& pLocal = H::Entities.GetLocal();
			if (pLocal && pRagdoll->m_iTeam() == pLocal->m_iTeamNum())
				return CALL_ORIGINAL(ecx);
		}

		ClearEffects(pRagdoll);

		pRagdoll->m_bBurning() = Vars::Visuals::Ragdolls::Effects.Value & (1 << 0);
		pRagdoll->m_bElectrocuted() = Vars::Visuals::Ragdolls::Effects.Value & (1 << 1);
		pRagdoll->m_bBecomeAsh() = Vars::Visuals::Ragdolls::Effects.Value & (1 << 2);
		pRagdoll->m_bDissolving() = Vars::Visuals::Ragdolls::Effects.Value & (1 << 3);
		pRagdoll->m_bGoldRagdoll() = Vars::Visuals::Ragdolls::Type.Value == 1;
		pRagdoll->m_bIceRagdoll() = Vars::Visuals::Ragdolls::Type.Value == 2;

		pRagdoll->m_vecForce() *= Vars::Visuals::Ragdolls::Force.Value;
		pRagdoll->m_vecForce().x *= Vars::Visuals::Ragdolls::ForceHorizontal.Value;
		pRagdoll->m_vecForce().y *= Vars::Visuals::Ragdolls::ForceHorizontal.Value;
		pRagdoll->m_vecForce().z *= Vars::Visuals::Ragdolls::ForceVertical.Value;
	}

	CALL_ORIGINAL(ecx);
}