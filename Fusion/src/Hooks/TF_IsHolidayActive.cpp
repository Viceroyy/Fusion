#include "../SDK/SDK.h"

// Credits to Myzarfin

enum EHoliday
{
	kHoliday_None = 0,		// must stay at zero for backwards compatibility
	kHoliday_TFBirthday,
	kHoliday_Halloween,
	kHoliday_Christmas,
	kHoliday_CommunityUpdate,
	kHoliday_EOTL,
	kHoliday_Valentines,
	kHoliday_MeetThePyro,
	kHoliday_FullMoon,
	kHoliday_HalloweenOrFullMoon,
	kHoliday_HalloweenOrFullMoonOrValentines,
	kHoliday_AprilFools,
	kHolidayCount,
};

MAKE_SIGNATURE(TF_IsHolidayActive, "client.dll", "48 83 EC ? 48 8B 05 ? ? ? ? 44 8B C9", 0x0);

MAKE_HOOK(TF_IsHolidayActive, S::TF_IsHolidayActive(), bool, __fastcall,
	int eHoliday)
{
	return eHoliday == kHoliday_HalloweenOrFullMoon ? true : CALL_ORIGINAL(eHoliday);
}