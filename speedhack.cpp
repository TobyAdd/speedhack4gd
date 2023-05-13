#include <windows.h>

typedef void* (__cdecl* fSharedDirector)();
typedef void* (__thiscall* fGetScheduler)(void*);
typedef void(__thiscall* fSetTimeScale)(void*, float);

fSharedDirector sharedDirector = NULL;
fGetScheduler getScheduler = NULL;
fSetTimeScale setTimeScale = NULL;

void speedHack(float speed)
{
    sharedDirector = (fSharedDirector)GetProcAddress(GetModuleHandle("libcocos2d.dll"), "?sharedDirector@CCDirector@cocos2d@@SAPAV12@XZ");
    getScheduler = (fGetScheduler)GetProcAddress(GetModuleHandle("libcocos2d.dll"), "?getScheduler@CCDirector@cocos2d@@UAEPAVCCScheduler@2@XZ");
    setTimeScale = (fSetTimeScale)GetProcAddress(GetModuleHandle("libcocos2d.dll"), "?setTimeScale@CCScheduler@cocos2d@@QAEXM@Z");

    void* director = sharedDirector();
    void* scheduler = getScheduler(director);

    setTimeScale(scheduler, speed);
}

DWORD WINAPI MainThread(LPVOID lpParam)
{
    speedHack(0.5f);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        CreateThread(0, 0x1000, MainThread, 0, 0, 0);
    }
    return TRUE;
}
