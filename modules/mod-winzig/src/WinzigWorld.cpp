#include "Config.h"
#include "WinzigWorld.h"

bool WinzigWorld::Enabled;

int WinzigWorld::StackSize;

float WinzigWorld::FiveStarRate;
float WinzigWorld::FourStarRate;

uint32 WinzigWorld::NPC;
uint32 WinzigWorld::StarterBox;
uint32 WinzigWorld::ClassicBox;
uint32 WinzigWorld::BurntBox;
uint32 WinzigWorld::FrozenBox;
uint32 WinzigWorld::ReagentPouch;
int WinzigWorld::CustomCurrency;

void WinzigWorld::SetInitialWorldSettings()
{
    Enabled = sConfigMgr->GetOption<bool>("Winzig.Enable", true);

    StackSize = sConfigMgr->GetOption<int>("Winzig.StackSize", 5);

    FiveStarRate = sConfigMgr->GetOption<float>("Winzig.FiveStarRate", 0.006);
    FourStarRate = sConfigMgr->GetOption<float>("Winzig.FourStarRate", 0.051);

    NPC = sConfigMgr->GetOption<uint32>("Winzig.NPC", 17249);
    StarterBox = sConfigMgr->GetOption<uint32>("Winzig.StarterBox", 5621798);
    ClassicBox = sConfigMgr->GetOption<uint32>("Winzig.ClassicBox", 5621799);
    BurntBox = sConfigMgr->GetOption<uint32>("Winzig.BurntBox", 5621800);
    FrozenBox = sConfigMgr->GetOption<uint32>("Winzig.FrozenBox", 5621801);
    ReagentPouch = sConfigMgr->GetOption<uint32>("Winzig.ReagentPouch", 2251094);
    CustomCurrency = sConfigMgr->GetOption<int>("Winzig.CustomCurrency", 37711);
}

void AddWinzigWorldScripts()
{
    new WinzigWorld();
}
