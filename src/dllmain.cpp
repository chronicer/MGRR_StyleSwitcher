#include "pch.h"

// #include "kiero.h"
#include "minhook/include/MinHook.h"
#include <d3d9.h>

#include "cheat/cheat.h"
#include "plugin.h"
#include "common.h"

// HANDLE GameProcess = GetCurrentProcess();
// DWORD base = cheat::GetBaseAddress(GameProcess);
// static HWND window = (HWND)injector::ReadMemory<DWORD>(base + 0x19D504C);

void OnEndSceneFunc()
{
    static bool init = false;
}

/*
void RenderTest()
{
    if (!Hw::GraphicDevice || *(bool*)(shared::base + 0x1B206F4) || !bIsForegroundWindow)
        return;

    Hw::GraphicDevice->Clear(1, new D3DRECT{ 0, 0, 100, 100 }, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 0, 0), 0.0f, 0);
}
*/

void DeviceResetAfter()
{
}

void plugin::OnStartup()
{
    plugin::OnGameStartup::Add(cheat::Initialize);
    // plugin::DrawingEvent::Add(RenderTest);
    plugin::OnEndScene::Add(OnEndSceneFunc);
    plugin::OnDeviceReset::After::Add(DeviceResetAfter);
    
    //plugin::OnTickEvent::Add(cheat::CustomEffectPlayer);

    plugin::UpdateEvent::Add(cheat::Update);
}