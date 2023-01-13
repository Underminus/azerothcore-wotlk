#ifndef _WINZIG_WORLD_H_
#define _WINZIG_WORLD_H_

#include <vector>
#include "ScriptMgr.h"

class WinzigWorld : public WorldScript
{
public:
    WinzigWorld() : WorldScript("WinzigWorld") {}

    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        SetInitialWorldSettings();
    }

    void SetInitialWorldSettings();

    static bool Enabled;

    static int StackSize;

    static float FiveStarRate;
    static float FourStarRate;

    static uint32 StarterBox;
    static uint32 ClassicBox;
    static uint32 BurntBox;
    static uint32 FrozenBox;
    static uint32 ReagentPouch;
    static uint32 NPC;
    static int CustomCurrency;
};

#endif // _WINZIG_WORLD_H_
