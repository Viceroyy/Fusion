// what is this for? - this is for removing the survey at the end of a match https://i.imgur.com/Y41j9er.png

#include "../SDK/SDK.h"

MAKE_SIGNATURE(CTFMatchSummary_OnTick, "client.dll", "40 55 57 41 55 48 8D 6C 24", 0x0);
MAKE_SIGNATURE(GTFGCClientSystem, "client.dll", "48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 8B 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24", 0x0)
// ^ this needs an interface
// Credits: mfed
MAKE_HOOK(CTFMatchSummary_OnTick, S::CTFMatchSummary_OnTick(), int, __fastcall,
    void* rcx)
{
    DWORD* surveyThing = (DWORD*)(S::GTFGCClientSystem() + 1960);
    if (surveyThing && *surveyThing & 2)
    {
        *surveyThing &= ~2;
    }

    return CALL_ORIGINAL(rcx);
}