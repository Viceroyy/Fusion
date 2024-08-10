#pragma once
#include "CBaseHandle.h"
#include "IClientEntity.h"
#include "CCollisionProperty.h"
#include "../Main/UtlVector.h"
#include "../Definitions.h"
#include "../../../Utils/NetVars/NetVars.h"
#include "../../../Utils/Signatures/Signatures.h"
#include "../../../Utils/Memory/Memory.h"

MAKE_SIGNATURE(CBaseEntity_SetAbsOrigin, "client.dll", "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 E8 ? ? ? ? F3 0F 10 83", 0x0);
MAKE_SIGNATURE(CBaseEntity_SetAbsAngles, "client.dll", "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B FA 48 8B D9 E8 ? ? ? ? F3 0F 10 83", 0x0);
MAKE_SIGNATURE(CBaseEntity_SetAbsVelocity, "client.dll", "48 89 5C 24 ? 57 48 83 EC ? F3 0F 10 81 ? ? ? ? 48 8B DA 0F 2E 02", 0x0);
MAKE_SIGNATURE(CBaseEntity_EstimateAbsVelocity, "client.dll", "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 E8 ? ? ? ? 48 3B D8", 0x0);
MAKE_SIGNATURE(CBaseEntity_CreateShadow, "client.dll", "48 89 5C 24 ? 57 48 83 EC ? 48 8B 41 ? 48 8B F9 48 83 C1 ? FF 90", 0x0);
MAKE_SIGNATURE(CBaseEntity_InvalidateBoneCache, "client.dll", "8B 05 ? ? ? ? FF C8 C7 81", 0x0);
MAKE_SIGNATURE(CBaseEntity_SetPredictionRandomSeed, "client.dll", "48 85 C9 75 ? C7 05 ? ? ? ? ? ? ? ? C3", 0x0);
MAKE_SIGNATURE(CBaseEntity_SetPredictionPlayer, "client.dll", "48 89 3D ? ? ? ? 66 0F 6E 87", 0x0);
MAKE_SIGNATURE(CBaseEntity_PhysicsRunThink, "client.dll", "4C 8B DC 49 89 73 ? 57 48 81 EC ? ? ? ? 8B 81", 0x0);

enum CollideType_t
{
	ENTITY_SHOULD_NOT_COLLIDE = 0,
	ENTITY_SHOULD_COLLIDE,
	ENTITY_SHOULD_RESPOND
};

typedef CHandle<CBaseEntity> EHANDLE;

#define MULTIPLAYER_BACKUP 90

class IInterpolatedVar;

class VarMapEntry_t
{
public:
	unsigned short type;
	unsigned short m_bNeedsToInterpolate;
	void* data;
	IInterpolatedVar* watcher;
};

struct VarMapping_t
{
	CUtlVector<VarMapEntry_t> m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};

enum thinkmethods_t
{
	THINK_FIRE_ALL_FUNCTIONS,
	THINK_FIRE_BASE_ONLY,
	THINK_FIRE_ALL_BUT_BASE,
};

class CTakeDamageInfo;
class CDmgAccumulator;
class CEntityMapData;
class C_Team;
class IClientVehicle;
class CNewParticleEffect;
class CDamageModifier;
struct EmitSound_t;
class CBaseAnimating;
class CBaseCombatCharacter;
class CBaseCombatWeapon;

class CBaseEntity : public IClientEntity
{
public:
	virtual                         ~CBaseEntity() = 0;

	virtual datamap_t*				GetDataDescMap() = 0;
	virtual void*					YouForgotToImplementOrDeclareClientClass() = 0;
	virtual datamap_t*				GetPredDescMap() = 0;

	virtual void					FireBullets(const FireBulletsInfo_t& info) = 0;
	virtual void					ModifyFireBulletsDamage(CTakeDamageInfo* dmgInfo) = 0;
	virtual bool					ShouldDrawUnderwaterBulletBubbles() = 0;
	virtual bool					ShouldDrawWaterImpacts(void) = 0;
	virtual bool					HandleShotImpactingWater(const FireBulletsInfo_t& info, const Vector& vecEnd, ITraceFilter* pTraceFilter, Vector* pVecTracerDest) = 0;
	virtual ITraceFilter*			GetBeamTraceFilter(void) = 0;
	virtual void					DispatchTraceAttack(const CTakeDamageInfo* info, const Vector& vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator = NULL) = 0;
	virtual void					TraceAttack(const CTakeDamageInfo* info, const Vector& vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator = NULL) = 0;
	virtual void					DoImpactEffect(trace_t& tr, int nDamageType) = 0;
	virtual void					MakeTracer(const Vector& vecTracerSrc, const trace_t& tr, int iTracerType) = 0;
	virtual int						GetTracerAttachment(void) = 0;
	virtual int						BloodColor() = 0;
	virtual const char*				GetTracerType() = 0;
	virtual void					Spawn(void) = 0;
	virtual void					SpawnClientEntity(void) = 0;
	virtual void					Precache(void) = 0;
	virtual void					Activate() = 0;
	virtual void					ParseMapData(CEntityMapData* mapData) = 0;
	virtual bool					KeyValue(const char* szKeyName, const char* szValue) = 0;
	virtual bool					KeyValue(const char* szKeyName, float flValue) = 0;
	virtual bool					KeyValue(const char* szKeyName, const Vector& vecValue) = 0;
	virtual bool					GetKeyValue(const char* szKeyName, char* szValue, int iMaxLen) = 0;
	virtual bool					Init(int entnum, int iSerialNum) = 0;
	virtual CBaseAnimating*			GetBaseAnimating() = 0;
	virtual void					SetClassname(const char* className) = 0;
	virtual void			        RecordToolMessage() = 0;
	virtual Vector					GetObserverCamOrigin(void) = 0;
	virtual bool					TestCollision(const Ray_t& ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual bool					TestHitboxes(const Ray_t& ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual float					GetAttackDamageScale(void) = 0;
	virtual void					ValidateModelIndex(void) = 0;
	virtual void					SetDormant(bool bDormant) = 0;
	virtual int				        GetEFlags() const = 0;
	virtual void			        SetEFlags(int iEFlags) = 0;
	virtual bool					ShouldSavePhysics() = 0;
	virtual void					OnSave() = 0;
	virtual void					OnRestore() = 0;
	virtual int						ObjectCaps(void) = 0;
	virtual int						Save(ISave& save) = 0;
	virtual int						Restore(IRestore& restore) = 0;
	virtual bool	                CreateVPhysics() = 0;
	virtual void	                VPhysicsDestroyObject(void) = 0;
	virtual void					VPhysicsUpdate(IPhysicsObject* pPhysics) = 0;
	virtual int						VPhysicsGetObjectList(IPhysicsObject** pList, int listMax) = 0;
	virtual bool					VPhysicsIsFlesh(void) = 0;
	virtual void					AddEntity(void) = 0;
	virtual const Vector&			GetPrevLocalOrigin() const = 0;
	virtual const Vector&			GetPrevLocalAngles() const = 0;
	virtual int						CalcOverrideModelIndex() = 0;
	virtual const Vector&			WorldAlignMins() const = 0;
	virtual const Vector&			WorldAlignMaxs() const = 0;
	virtual const Vector&			WorldSpaceCenter() const = 0;
	virtual void					ComputeWorldSpaceSurroundingBox(Vector* pVecWorldMins, Vector* pVecWorldMaxs) = 0;
	virtual float					GetHealthBarHeightOffset() const = 0;
	virtual SolidType_t				GetSolid(void) const = 0;
	virtual int			 			GetSolidFlags(void) const = 0;
	virtual bool					GetAttachment(int number, Vector& origin) = 0;
	virtual bool					GetAttachmentVelocity(int number, Vector& originVel, Quaternion& angleVel) = 0;
	virtual C_Team*					GetTeam(void) const = 0;
	virtual int						GetTeamNumber(void) const = 0;
	virtual void					ChangeTeam(int iTeamNum) = 0;
	virtual int						GetRenderTeamNumber(void) = 0;
	virtual bool					InSameTeam(const CBaseEntity* pEntity) const = 0;
	virtual bool					InLocalTeam(void) = 0;
	virtual bool					IsValidIDTarget(void) = 0;
	virtual const char*				GetIDString(void) = 0;
	virtual void                    ModifyEmitSoundParams(EmitSound_t& params) = 0;
	virtual bool                    InitializeAsClientEntity(const char* pszModelName, int renderGroup) = 0; //RenderGroup_t
	virtual void					Simulate() = 0;
	virtual IClientVehicle*			GetClientVehicle() = 0;
	virtual void					GetAimEntOrigin(IClientEntity* pAttachedTo, Vector* pAbsOrigin, Vector* pAbsAngles) = 0;
	virtual const Vector&			GetOldOrigin() = 0;
	virtual int			            GetRenderGroup() = 0; //RenderGroup_t
	virtual void					GetToolRecordingState(KeyValues* msg) = 0;
	virtual void					CleanupToolRecordingState(KeyValues* msg) = 0;
	virtual CollideType_t			GetCollideType(void) = 0;
	virtual void					UpdateVisibility() = 0;
	virtual bool					IsSelfAnimating() = 0;
	virtual	void					OnLatchInterpolatedVariables(int flags) = 0;
	virtual CStudioHdr*				OnNewModel() = 0;
	virtual void					OnNewParticleEffect(const char* pszParticleName, CNewParticleEffect* pNewParticleEffect) = 0;
	virtual void					ResetLatched() = 0;
	virtual bool					Interpolate(float currentTime) = 0;
	virtual bool					IsSubModel(void) = 0;
	virtual void					CreateLightEffects(void) = 0;
	virtual void					Clear(void) = 0;
	virtual int						DrawBrushModel(bool bTranslucent, int nFlags, bool bTwoPass) = 0;
	virtual float					GetTextureAnimationStartTime() = 0;
	virtual void					TextureAnimationWrapped() = 0;
	virtual void					SetNextClientThink(float nextThinkTime) = 0;
	virtual void					SetHealth(int iHealth) = 0;
	virtual int						GetHealth() const = 0;
	virtual int						GetMaxHealth() const = 0;
	virtual bool					IsVisibleToTargetID(void) const = 0;
	virtual bool					IsHealthBarVisible(void) const = 0;
	virtual ShadowType_t			ShadowCastType() = 0;
	virtual bool					ShouldReceiveProjectedTextures(int flags) = 0;
	virtual void					AddDecal(const Vector& rayStart, const Vector& rayEnd, const Vector& decalCenter, int hitbox, int decalIndex, bool doTrace, trace_t& tr, int maxLODToDecal = -1) = 0;
	virtual void					AddColoredDecal(const Vector& rayStart, const Vector& rayEnd, const Vector& decalCenter, int hitbox, int decalIndex, bool doTrace, trace_t& tr, Color_t cColor, int maxLODToDecal = -1) = 0;
	virtual bool					IsClientCreated(void) const = 0;
	virtual void					UpdateOnRemove(void) = 0;
	virtual void					SUB_Remove(void) = 0;
	virtual void					SetPredictable(bool state) = 0;
	virtual int						RestoreData(const char* context, int slot, int type) = 0;
	virtual char const*				DamageDecal(int bitsDamageType, int gameMaterial) = 0;
	virtual void					DecalTrace(trace_t* pTrace, char const* decalName) = 0;
	virtual void					ImpactTrace(trace_t* pTrace, int iDamageType, const char* pCustomImpactName) = 0;
	virtual bool					ShouldPredict(void) = 0;
	virtual void					Think(void) = 0;
	virtual bool			        CanBePoweredUp(void) = 0;
	virtual bool			        AttemptToPowerup(int iPowerup, float flTime, float flAmount = 0, CBaseEntity* pAttacker = NULL, CDamageModifier* pDamageModifier = NULL) = 0;
	virtual bool			        IsCurrentlyTouching(void) const = 0;
	virtual void			        StartTouch(CBaseEntity* pOther) = 0;
	virtual void			        Touch(CBaseEntity* pOther) = 0;
	virtual void			        EndTouch(CBaseEntity* pOther) = 0;
	virtual unsigned int	        PhysicsSolidMaskForEntity(void) const = 0;
	virtual void					PhysicsSimulate(void) = 0;
	virtual bool					IsAlive(void) = 0;
	virtual bool					IsPlayer(void) const = 0;
	virtual bool					IsBaseCombatCharacter(void) = 0;
	virtual CBaseCombatCharacter*	MyCombatCharacterPointer(void) = 0;
	virtual bool					IsNPC(void) = 0;
	virtual bool					IsNextBot() = 0;
	virtual bool					IsBaseObject(void) const = 0;
	virtual bool					IsBaseCombatWeapon(void) const = 0;
	virtual CBaseCombatWeapon*		MyCombatWeaponPointer() = 0;
	virtual bool					IsCombatItem(void) const = 0;
	virtual bool					IsBaseTrain(void) const = 0;
	virtual Vector			        EyePosition(void) = 0;
	virtual const Vector&			EyeAngles(void) = 0;
	virtual const Vector&			LocalEyeAngles(void) = 0;
	virtual Vector		            EarPosition(void) = 0;
	virtual bool		            ShouldCollide(int collisionGroup, int contentsMask) const = 0;
	virtual const Vector&			GetViewOffset() const = 0;
	virtual void		            SetViewOffset(const Vector& v) = 0;
	virtual void                    ClientAdjustStartSoundParams(void* unk) = 0; //EmitSound_t
	virtual void                    ClientAdjustStartSoundParams2(void* unk) = 0; //StartSoundParams_t
	virtual bool                    ShouldInterpolate() = 0;
	virtual unsigned char           GetClientSideFade() = 0;
	virtual void                    BoneMergeFastCullBloat(Vector& localMins, Vector& localMaxs, const Vector& thisEntityMins, const Vector& thisEntityMaxs) const = 0;
	virtual bool					OnPredictedEntityRemove(bool isbeingremoved, CBaseEntity* predicted) = 0;
	virtual CBaseEntity*			GetShadowUseOtherEntity(void) const = 0;
	virtual void					SetShadowUseOtherEntity(CBaseEntity* pEntity) = 0;
	virtual bool					AddRagdollToFadeQueue(void) = 0;
	virtual bool					IsDeflectable() = 0;
	virtual int						GetStudioBody(void) = 0;
	virtual void                    PerformCustomPhysics(Vector* pNewPosition, Vector* pNewVelocity, Vector* pNewAngles, Vector* pNewAngVelocity) = 0;
	virtual bool                    ValidateEntityAttachedToPlayer(bool& bShouldRetry) = 0;

public:
	NETVAR(m_flAnimTime, float, "CBaseEntity", "m_flAnimTime");
	NETVAR(m_flSimulationTime, float, "CBaseEntity", "m_flSimulationTime");
	NETVAR(m_ubInterpolationFrame, int, "CBaseEntity", "m_ubInterpolationFrame");
	NETVAR(m_vecOrigin, Vec3, "CBaseEntity", "m_vecOrigin");
	NETVAR(m_angRotation, Vec3, "CBaseEntity", "m_angRotation");
	NETVAR(m_nModelIndex, int, "CBaseEntity", "m_nModelIndex");
	NETVAR(m_fEffects, int, "CBaseEntity", "m_fEffects");
	NETVAR(m_nRenderMode, int, "CBaseEntity", "m_nRenderMode");
	NETVAR(m_nRenderFX, int, "CBaseEntity", "m_nRenderFX");
	NETVAR(m_clrRender, Color_t, "CBaseEntity", "m_clrRender");
	NETVAR(m_iTeamNum, int, "CBaseEntity", "m_iTeamNum");
	NETVAR(m_CollisionGroup, int, "CBaseEntity", "m_CollisionGroup");
	NETVAR(m_flElasticity, float, "CBaseEntity", "m_flElasticity");
	NETVAR(m_flShadowCastDistance, float, "CBaseEntity", "m_flShadowCastDistance");
	NETVAR(m_hOwnerEntity, EHANDLE, "CBaseEntity", "m_hOwnerEntity");
	NETVAR(m_hEffectEntity, EHANDLE, "CBaseEntity", "m_hEffectEntity");
	NETVAR(moveparent, int, "CBaseEntity", "moveparent");
	NETVAR(m_iParentAttachment, int, "CBaseEntity", "m_iParentAttachment");
	NETVAR(m_Collision, CCollisionProperty*, "CBaseEntity", "m_Collision");
	NETVAR(m_vecMinsPreScaled, Vec3, "CBaseEntity", "m_vecMinsPreScaled");
	NETVAR(m_vecMaxsPreScaled, Vec3, "CBaseEntity", "m_vecMaxsPreScaled");
	NETVAR(m_vecMins, Vec3, "CBaseEntity", "m_vecMins");
	NETVAR(m_vecMaxs, Vec3, "CBaseEntity", "m_vecMaxs");
	NETVAR(m_nSolidType, int, "CBaseEntity", "m_nSolidType");
	NETVAR(m_usSolidFlags, int, "CBaseEntity", "m_usSolidFlags");
	NETVAR(m_nSurroundType, int, "CBaseEntity", "m_nSurroundType");
	NETVAR(m_triggerBloat, int, "CBaseEntity", "m_triggerBloat");
	NETVAR(m_bUniformTriggerBloat, bool, "CBaseEntity", "m_bUniformTriggerBloat");
	NETVAR(m_vecSpecifiedSurroundingMinsPreScaled, Vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMinsPreScaled");
	NETVAR(m_vecSpecifiedSurroundingMaxsPreScaled, Vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMaxsPreScaled");
	NETVAR(m_vecSpecifiedSurroundingMins, Vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMins");
	NETVAR(m_vecSpecifiedSurroundingMaxs, Vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMaxs");
	NETVAR(m_iTextureFrameIndex, int, "CBaseEntity", "m_iTextureFrameIndex");
	NETVAR(m_PredictableID, int, "CBaseEntity", "m_PredictableID");
	NETVAR(m_bIsPlayerSimulated, bool, "CBaseEntity", "m_bIsPlayerSimulated");
	NETVAR(m_bSimulatedEveryTick, bool, "CBaseEntity", "m_bSimulatedEveryTick");
	NETVAR(m_bAnimatedEveryTick, bool, "CBaseEntity", "m_bAnimatedEveryTick");
	NETVAR(m_bAlternateSorting, bool, "CBaseEntity", "m_bAlternateSorting");
	NETVAR(m_nModelIndexOverrides, void*, "CBaseEntity", "m_nModelIndexOverrides");
	NETVAR(movetype, int, "CBaseEntity", "movetype");
	
	NETVAR_OFF(m_flOldSimulationTime, float, "CBaseEntity", "m_flSimulationTime", 4);

	//VIRTUAL(UpdateVisibility, void, void(__thiscall*)(CBaseEntity*), this, 91);

	Vec3 GetCenter()
	{
		return m_vecOrigin() + Vec3(0, 0, (m_vecMins().z + m_vecMaxs().z) / 2);
	}

	Vec3 GetRenderCenter()
	{
		Vec3 vMin = {}, vMax = {};
		GetRenderBounds(vMin, vMax);
		return GetRenderOrigin() + Vec3(0.f, 0.f, (vMin.z + vMax.z) / 2);
	}

	bool IsInValidTeam(int* pTeamNumOut = nullptr)
	{
		int nTeamNum = m_iTeamNum();

		switch (nTeamNum)
		{
		case TF_TEAM_RED:
		case TF_TEAM_BLUE:
		{
			if (pTeamNumOut)
				*pTeamNumOut = nTeamNum;

			return true;
		}

		default: return false;
		}
	}

	CBaseEntity* GetMoveParent()
	{
		static int nOffset = U::NetVars.GetNetVar("CBaseEntity", "moveparent") - 8;
		auto m_pMoveParent = reinterpret_cast<EHANDLE*>(std::uintptr_t(this) + nOffset);

		if (!m_pMoveParent)
			return nullptr;

		return m_pMoveParent->Get();
	}

	CBaseEntity* NextMovePeer()
	{
		static int nOffset = U::NetVars.GetNetVar("CBaseEntity", "moveparent") - 16;
		auto m_pMovePeer = reinterpret_cast<EHANDLE*>(std::uintptr_t(this) + nOffset);

		if (!m_pMovePeer)
			return nullptr;

		return m_pMovePeer->Get();
	}

	CBaseEntity* FirstMoveChild()
	{
		static int nOffset = U::NetVars.GetNetVar("CBaseEntity", "moveparent") - 24;
		auto m_pMoveChild = reinterpret_cast<EHANDLE*>(std::uintptr_t(this) + nOffset);

		if (!m_pMoveChild)
			return nullptr;

		return m_pMoveChild->Get();
	}

	void SetAbsOrigin(const Vec3& absOrigin)
	{
		S::CBaseEntity_SetAbsOrigin.As<void(__fastcall*)(void*, const Vec3&)>()(this, absOrigin);
	}

	void SetAbsAngles(const Vec3& absAngles)
	{
		S::CBaseEntity_SetAbsAngles.As<void(__fastcall*)(void*, const Vec3&)>()(this, absAngles);
	}

	void SetAbsVelocity(const Vec3& vecAbsVelocity)
	{
		S::CBaseEntity_SetAbsVelocity.As<void(__thiscall*)(void*, const Vec3&)>()(this, vecAbsVelocity);
	}

	void EstimateAbsVelocity(Vec3& vel)
	{
		S::CBaseEntity_EstimateAbsVelocity.As<void(__thiscall*)(void*, Vec3&)>()(this, vel);
	}

	Vec3 GetAbsVelocity()
	{
		Vec3 vOut;
		EstimateAbsVelocity(vOut);
		return vOut;
	}

	void CreateShadow()
	{
		S::CBaseEntity_CreateShadow.As<void(__thiscall*)(void*)>()(this);
	}

	void InvalidateBoneCache()
	{
		S::CBaseEntity_InvalidateBoneCache.As<void(__fastcall*)(void*)>()(this);
	}

	static void SetPredictionRandomSeed(const CUserCmd* cmd)
	{
		S::CBaseEntity_SetPredictionRandomSeed.As<void(*)(const CUserCmd*)>()(cmd);
	}

	static void SetPredictionPlayer(CBasePlayer* player)
	{
		*reinterpret_cast<CBasePlayer**>(*reinterpret_cast<DWORD*>(S::CBaseEntity_SetPredictionPlayer() + 0x03) + S::CBaseEntity_SetPredictionPlayer() + 0x07) = player;
	}

	inline bool PhysicsRunThink(thinkmethods_t thinkMethod = THINK_FIRE_ALL_FUNCTIONS)
	{
		return S::CBaseEntity_PhysicsRunThink.As<bool(__thiscall*)(void*, thinkmethods_t)>()(this, thinkMethod);
	}
};