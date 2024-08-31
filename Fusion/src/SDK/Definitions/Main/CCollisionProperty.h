#pragma once
#include "CGameTrace.h"
#include "../Types.h"
#include "../../../Utils/Signatures/Signatures.h"

MAKE_SIGNATURE(CCollisionPropert_SetCollisionBounds, "client.dll", "48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 41 56 48 81 EC ? ? ? ? F2 0F 10 02", 0x0);
MAKE_SIGNATURE(CCollisionProperty_CalcNearestPoint, "client.dll", "48 89 5C 24 ? 57 48 83 EC ? 49 8B D8 48 8B F9 4C 8D 44 24", 0x0);

struct model_t;

class ICollideable
{
public:
	virtual void* GetEntityHandle() = 0;
	virtual const Vec3& OBBMinsPreScaled() const = 0;
	virtual const Vec3& OBBMaxsPreScaled() const = 0;
	virtual const Vec3& OBBMins() const = 0;
	virtual const Vec3& OBBMaxs() const = 0;
	virtual void			WorldSpaceTriggerBounds(Vec3* pVecWorldMins, Vec3* pVecWorldMaxs) const = 0;
	virtual bool			TestCollision(const Ray_t& ray, unsigned int fContentsMask, CGameTrace& tr) = 0;
	virtual bool			TestHitboxes(const Ray_t& ray, unsigned int fContentsMask, CGameTrace& tr) = 0;
	virtual int				GetCollisionModelIndex() = 0;
	virtual const model_t* GetCollisionModel() = 0;
	virtual const Vec3& GetCollisionOrigin() const = 0;
	virtual const Vec3& GetCollisionAngles() const = 0;
	virtual const matrix3x4& CollisionToWorldTransform() const = 0;
	virtual SolidType_t		GetSolid() const = 0;
	virtual int				GetSolidFlags() const = 0;
	virtual void* GetIClientUnknown() = 0;
	virtual int				GetCollisionGroup() const = 0;
	virtual void			WorldSpaceSurroundingBounds(Vec3* pVecMins, Vec3* pVecMaxs) = 0;
	virtual bool			ShouldTouchTrigger(int triggerSolidFlags) const = 0;
	virtual const matrix3x4* GetRootParentToWorldTransform() const = 0;
};

class CCollisionProperty : public ICollideable
{
public:
	void SetCollisionBounds(const Vec3& mins, const Vec3& maxs)
	{
		static auto func = S::CCollisionPropert_SetCollisionBounds.As<void(__fastcall*)(CCollisionProperty*, const Vec3&, const Vec3&)>();
		func(this, mins, maxs);
	}

	void CalcNearestPoint(const Vec3& vecWorldPt, Vec3* pVecNearestWorldPt)
	{
		static auto func = S::CCollisionProperty_CalcNearestPoint.As<void(__fastcall*)(CCollisionProperty*, const Vec3&, Vec3*)>();
		func(this, vecWorldPt, pVecNearestWorldPt);
	}
};