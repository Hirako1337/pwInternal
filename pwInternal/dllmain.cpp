// THIS INTERNAL SOURCE RELEASED BY HIRAKO SHINIJI FOR A GROUP OF PEOPLE WHOS ASKING ABOUT HOW TO INJECT CODE INTO THE GAMEASSEMBLY.DLL MODULE OF PIXEL WURLDS.
// I HOPE MY RELEASE IS HELPFUL FOR THEIR SHIT.

#include "pch.h"
#include <detours.h>
#include <string>
#include <iostream>
#include <random>


using namespace std;
#pragma warning (disable : 4996)

const char* Module = "GameAssembly";

uintptr_t BaseModule;

typedef void(__fastcall* _OnGui)(void);
_OnGui OnGui;

typedef void(__fastcall* _ChatMessage)(string message, int time, string channelType, string nick, string userID, string channel);
_ChatMessage ChatMessage;

typedef void(__fastcall* _isPlayerDead)(bool);
_isPlayerDead isPlayerDead;

typedef void(__fastcall* _pLevel)(string);
_pLevel playerLevel;

#define il2cpp_OnGUI 0x1974E80;
#define il2cpp_fps 0x1974CC0;
#define il2cpp_SubmitPrivateChatMessage 0x1198460;
#define il2cpp_isPlayerDead 0xE94730;
#define il2cpp_pLevel 0xE946C0;
HMODULE GameAssembly = GetModuleHandle(TEXT("GameAssembly.dll"));

uintptr_t hkgui;
uintptr_t hklog;
uintptr_t hkdie;
uintptr_t hk_usr;

std::string tag()
{
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(str.begin(), str.end(), generator);

    return str.substr(0, 5);    // creates random 5 letters as tag.     
}
void il2cpp_OnGUI_Hook(void) {

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)hkgui, &il2cpp_OnGUI_Hook);
    DetourTransactionCommit(); 

    OnGui();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)hkgui, &il2cpp_OnGUI_Hook);
    DetourTransactionCommit();
}
bool PlayerStatus_Alive_Hook(bool a1) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)hkdie, PlayerStatus_Alive_Hook);
    DetourTransactionCommit();

    isPlayerDead(a1);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)hkdie, PlayerStatus_Alive_Hook);
    DetourTransactionCommit();
    return a1;
}
void get_player_level(int xp) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)hk_usr, get_player_level);
    DetourTransactionCommit();

    int playerXP = xp;

   
    printf("\nYour Current Level (XP): %d");

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)hk_usr, get_player_level);
    DetourTransactionCommit();
    
}
void il2cpp_SendLogMessage_Hook(string message, int time, string channelType, string nick = "", string userID = "", string channel = "") {

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)hklog, &il2cpp_OnGUI_Hook);
    DetourTransactionCommit();
    std::cout << message << endl;
    ChatMessage(message,time,channelType,nick,userID,channel);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)hklog, &il2cpp_OnGUI_Hook);
    DetourTransactionCommit();
}
DWORD WINAPI Main(LPVOID a1) {
    printf("pwInternal brought to you by (c) Hirako Shiniji");
    get_player_level(0);
    while (1) {
        string data="";
        //memcpy(Gui_Log, &data, sizeof(Gui_Log));
        //printf("\n[Internal] %s", data); long usage crashes the game
        
    }
    return 0;
}
uint32_t BaseGame;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        BaseModule= (uintptr_t)GetModuleHandle(TEXT("GameAssembly.dll"));
        BaseGame = (uintptr_t)GetModuleHandle(NULL);
        hkgui = BaseModule + il2cpp_OnGUI;
        hklog = BaseModule +  il2cpp_SubmitPrivateChatMessage;
        hkdie = BaseModule + il2cpp_isPlayerDead;
        hk_usr = BaseModule + il2cpp_pLevel;
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)hkgui, &il2cpp_OnGUI_Hook);
        DetourAttach(&(PVOID&)hklog, &il2cpp_SendLogMessage_Hook);
        DetourAttach(&(PVOID&)hk_usr, &get_player_level);
        //DetourAttach(&(PVOID&)hkdie, &PlayerStatus_Alive_Hook); unstable
        DetourTransactionCommit();
        // so sorry for my messy stuff.
        CreateThread(0, 0, Main, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

