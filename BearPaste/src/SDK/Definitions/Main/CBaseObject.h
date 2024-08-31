#pragma once
#include "CBaseCombatCharacter.h"

class CBaseObject : public CBaseCombatCharacter
{
public:
	NETVAR(m_iHealth, int, "CBaseObject", "m_iHealth");
	NETVAR(m_iMaxHealth, int, "CBaseObject", "m_iMaxHealth");
	NETVAR(m_bHasSapper, bool, "CBaseObject", "m_bHasSapper");
	NETVAR(m_iObjectType, int, "CBaseObject", "m_iObjectType");
	NETVAR(m_bBuilding, bool, "CBaseObject", "m_bBuilding");
	NETVAR(m_bPlacing, bool, "CBaseObject", "m_bPlacing");
	NETVAR(m_bCarried, bool, "CBaseObject", "m_bCarried");
	NETVAR(m_bCarryDeploy, bool, "CBaseObject", "m_bCarryDeploy");
	NETVAR(m_bMiniBuilding, bool, "CBaseObject", "m_bMiniBuilding");
	NETVAR(m_flPercentageConstructed, float, "CBaseObject", "m_flPercentageConstructed");
	NETVAR(m_fObjectFlags, int, "CBaseObject", "m_fObjectFlags");
	NETVAR(m_hBuiltOnEntity, EHANDLE, "CBaseObject", "m_hBuiltOnEntity");
	NETVAR(m_bDisabled, bool, "CBaseObject", "m_bDisabled");
	NETVAR(m_hBuilder, EHANDLE, "CBaseObject", "m_hBuilder");
	NETVAR(m_vecBuildMaxs, Vec3, "CBaseObject", "m_vecBuildMaxs");
	NETVAR(m_vecBuildMins, Vec3, "CBaseObject", "m_vecBuildMins");
	NETVAR(m_iDesiredBuildRotations, int, "CBaseObject", "m_iDesiredBuildRotations");
	NETVAR(m_bServerOverridePlacement, bool, "CBaseObject", "m_bServerOverridePlacement");
	NETVAR(m_iUpgradeLevel, int, "CBaseObject", "m_iUpgradeLevel");
	NETVAR(m_iUpgradeMetal, int, "CBaseObject", "m_iUpgradeMetal");
	NETVAR(m_iUpgradeMetalRequired, int, "CBaseObject", "m_iUpgradeMetalRequired");
	NETVAR(m_iHighestUpgradeLevel, int, "CBaseObject", "m_iHighestUpgradeLevel");
	NETVAR(m_iObjectMode, int, "CBaseObject", "m_iObjectMode");
	NETVAR(m_bDisposableBuilding, bool, "CBaseObject", "m_bDisposableBuilding");
	NETVAR(m_bWasMapPlaced, bool, "CBaseObject", "m_bWasMapPlaced");
	NETVAR(m_bPlasmaDisable, bool, "CBaseObject", "m_bPlasmaDisable");

	VIRTUAL(GetNumBuildPoints, int, int(__fastcall*)(void*), IHasBuildPoints(), 0);

	void* IHasBuildPoints()
	{
		static int nOffset = U::NetVars.GetNetVar("CBaseObject", "m_iUpgradeLevel") - 340;
		return reinterpret_cast<void*>(std::uintptr_t(this) + nOffset);
	};

	bool GetBuildPoint(int iPoint, Vec3& vecOrigin, Vec3& vecAngles)
	{
		void* pPoints = IHasBuildPoints();
		return reinterpret_cast<bool(__fastcall*)(void*, int, Vec3&, Vec3&)>(U::Memory.GetVFunc(pPoints, 1))(pPoints, iPoint, vecOrigin, vecAngles);
	}

	__inline int GetBuildPointAttachmentIndex(int iPoint)
	{
		void* pPoints = IHasBuildPoints();
		return reinterpret_cast<int(__fastcall*)(void*, int)>(U::Memory.GetVFunc(pPoints, 2))(pPoints, iPoint);
	}

	bool IsDisabled()
	{
		return m_bDisabled() || m_bHasSapper();
	}
};

class CObjectSentrygun : public CBaseObject
{
public:
	NETVAR(m_iAmmoShells, int, "CObjectSentrygun", "m_iAmmoShells");
	NETVAR(m_iAmmoRockets, int, "CObjectSentrygun", "m_iAmmoRockets");
	NETVAR(m_iState, int, "CObjectSentrygun", "m_iState");
	NETVAR(m_bPlayerControlled, bool, "CObjectSentrygun", "m_bPlayerControlled");
	NETVAR(m_nShieldLevel, int, "CObjectSentrygun", "m_nShieldLevel");
	NETVAR(m_bShielded, bool, "CObjectSentrygun", "m_bShielded");
	NETVAR(m_hEnemy, EHANDLE, "CObjectSentrygun", "m_hEnemy");
	NETVAR(m_hAutoAimTarget, EHANDLE, "CObjectSentrygun", "m_hAutoAimTarget");
	NETVAR(m_iKills, int, "CObjectSentrygun", "m_iKills");
	NETVAR(m_iAssists, int, "CObjectSentrygun", "m_iAssists");

	__inline int MaxAmmoShells()
	{
		if (m_iUpgradeLevel() == 1 || m_bMiniBuilding())
			return 150;
		else
			return 200;
	}

	__inline void GetAmmoCount(int& iShells, int& iMaxShells, int& iRockets, int& iMaxRockets)
	{
		const bool bIsMini = m_bMiniBuilding();

		iShells = m_iAmmoShells();
		iMaxShells = MaxAmmoShells();
		iRockets = bIsMini ? 0 : m_iAmmoRockets();
		iMaxRockets = (bIsMini || m_iUpgradeLevel() < 3) ? 0 : 20;
	}
};

class CObjectDispenser : public CBaseObject
{
public:
	NETVAR(m_iState, int, "CObjectDispenser", "m_iState");
	NETVAR(m_iAmmoMetal, int, "CObjectDispenser", "m_iAmmoMetal");
	NETVAR(m_iMiniBombCounter, int, "CObjectDispenser", "m_iMiniBombCounter");
};

class CObjectTeleporter : public CBaseObject
{
public:
	NETVAR(m_iState, int, "CObjectTeleporter", "m_iState");
	NETVAR(m_flRechargeTime, float, "CObjectTeleporter", "m_flRechargeTime");
	NETVAR(m_flCurrentRechargeDuration, float, "CObjectTeleporter", "m_flCurrentRechargeDuration");
	NETVAR(m_iTimesUsed, int, "CObjectTeleporter", "m_iTimesUsed");
	NETVAR(m_flYawToExit, float, "CObjectTeleporter", "m_flYawToExit");
	NETVAR(m_bMatchBuilding, bool, "CObjectTeleporter", "m_bMatchBuilding");

	inline float GetRechargeTime()
	{
		if (m_iState() != TELEPORTER_STATE_RECHARGING)
			return 0.0f;

		return m_flRechargeTime() - I::GlobalVars->curtime + 0.05f;
	}
};