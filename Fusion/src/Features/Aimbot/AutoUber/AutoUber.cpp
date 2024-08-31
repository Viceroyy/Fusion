#include "AutoUber.h"
#include "../../Players/PlayerUtils.h"
// todo: add two modes, smart and manual.
// code is probably shit

struct GlobalVars
{
    float curtime;
};

GlobalVars g_GlobalVars;

void UpdateCurrentTime()
{
    g_GlobalVars.curtime += 0.001f;
}

constexpr int FESTIVE_UBERSAW_WEAPON_ID = 1003;
constexpr int THE_UBERSAW_WEAPON_ID = 37;

int GetPlayerClass(CTFPlayer* pPlayer)
{
    return TF_CLASS_MEDIC;
}

int GetPlayerCurrentWeaponID(CTFPlayer* pPlayer)
{
    return 0;
}

void CAutoUber::Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
{
    static float lastUberTime = 0.0f;

    UpdateCurrentTime();

    if (!pLocal || !pWeapon)
        return;

    if (GetPlayerClass(pLocal) == TF_CLASS_MEDIC)
    {
        EWeaponType currentWeaponType = G::WeaponType;

        if (currentWeaponType == EWeaponType::MELEE)
        {
            return;
        }

        int currentWeaponID = GetPlayerCurrentWeaponID(pLocal);

        if (currentWeaponID != FESTIVE_UBERSAW_WEAPON_ID && currentWeaponID != THE_UBERSAW_WEAPON_ID)
        {
            // shit for vars probably dosent work
            bool autoUberEnabled = Vars::AutoUber::AutoUberEnabled.Value; 
            float autoUberIntervalMs = Vars::AutoUber::AutoUberIntervalMs.Value; 

            if (autoUberEnabled) 
            {
                float currentTime = g_GlobalVars.curtime;
                float autoUberInterval = autoUberIntervalMs / 1000.0f; // convert ms to seconds

                if (currentTime - lastUberTime >= autoUberInterval)
                {
                    pCmd->buttons |= IN_ATTACK2;

                    lastUberTime = currentTime;
                }
            }
        }
    }
}
