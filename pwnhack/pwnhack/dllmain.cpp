// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include "memory.cpp"
#include "proc.cpp"
#include "hooks.cpp"
#include "vector"

//values from cheat engine
void teleport(uintptr_t procBase, float x, float y, float z)
{
    *(float *)memory::findAddr(procBase + 0x018FFDE4, {0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x90}) = x;
    *(float *)memory::findAddr(procBase + 0x018FFDE4, {0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x94}) = y;
    *(float *)memory::findAddr(procBase + 0x018FFDE4, {0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x98}) = z;
}


DWORD WINAPI HackThread(HMODULE hModule)
{
    //console
    AllocConsole();
    FILE *f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    //Temporay, replace with our chat function later   
    std::cout << "  [DELETE] to quitt\n";   
    std::cout << "  [Numpad 1] for infinite mana            [Toggle]\n";
    std::cout << "  [Numpad 2] for Infinite Ammo            [Toggle]\n";
    std::cout << "  [Numpad 3] for infinite coins/items     [Toggle]\n";
    std::cout << "  [Numpad 4] for Teleport to Fire Spell\n";
    std::cout << "  [Numpad 5] for Teleport to Gun Shop\n";    
    std::cout << "  [Numpad 6] for Speed Hack               [Toggle]\n";
    std::cout << "  [Numpad 7] for Zero Damage              [Toggle]\n";
    std::cout << "  [Numpad 8] Inject Chat(Type fire)\n";
    std::cout << "  [Numpad 9] Add 500 coins\n";
    
    DWORD procId = GetProcId(L"PwnAdventure3-Win32-Shipping.exe");
    uintptr_t procBase = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"GameLogic.dll");   

    bool bAmmo = false, bSpeed = false, bMana = false, bItems = false, bDamage=false, bPistol=false;

    while (true)
    {
        // exit
        if (GetAsyncKeyState(VK_DELETE) & 1)
            break;

        // Infinite mana NOP
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bMana = !bMana;

            if (bMana)
            {
                std::cout << "[+] Enabling infinite mana\n";
                //nop bytes which update mana
                memory::nop((BYTE *)(moduleBase + 0x525C7), 6);
            }
            else
            {
                std::cout << "[-] Disabling infinite mana\n";
                //restore original bytes
                memory::patch((BYTE *)(moduleBase + 0x525C7), (BYTE *)"\x89\x86\xbc\x00\x00\x00", 6);
            }
        }

        // Infinite ammo NOP
        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bAmmo = !bAmmo;

            if (bAmmo)
            {
                std::cout << "[+] Enabling infinite ammo\n";
                memory::nop((BYTE *)(moduleBase + 0x52396), 3);
            }
            else
            {
                std::cout << "[-] Disabling infinite ammo\n";
                // Look at IDA Hex View to find original hex values for patching
                memory::patch((BYTE *)(moduleBase + 0x52396), (BYTE *)"\x89\x48\x1c", 3);
            }
        }

        // Infinite items NOP
        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bItems = !bItems;

            if (bItems)
            {
                std::cout << "[+] Enabling infinite coins/items\n";
                memory::nop((BYTE *)(moduleBase + 0x52217), 3);
            }
            else
            {
                std::cout << "[-] Disabling infinite coins/items\n";
                memory::patch((BYTE *)(moduleBase + 0x52217), (BYTE *)"\x89\x48\x18", 3);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD4) & 1)
        {
            // Teleport to the Fire Spell
            std::cout << "[+] Teleporting to Fire Spell Book\n";
            teleport(procBase, -43644.0f, -56041.0f, 309.0f);
        }

        if (GetAsyncKeyState(VK_NUMPAD5) & 1)
        {
            // Teleport to the Gun Shop
            std::cout << "[+] Teleporting to Gun Shop\n";
            teleport(procBase, -37496.0f, -18540.0f, 2514.0f);
        }

        //Will add more waypoints

        if (GetAsyncKeyState(VK_NUMPAD6) & 1)
        {
            bSpeed = !bSpeed;
            //GetWalkingSpeed function address in GameLogic.dll
            DWORD speedHookAddress = moduleBase + 0x4FF90;
            int speedHookLength = 6;
            if (bSpeed)
            {
                //address in real function to jump back to after our code
                speedJmpBackAddr = speedHookAddress + speedHookLength;
                std::cout << "[-] Hooking into walking speed method\n";
                walkSpeed = 2000.0f;
                //our code, in hooks.cpp
                if (memory::hook((void *)speedHookAddress, speedHook, speedHookLength))
                {
                    std::cout << "[+] Enabled speed hack\n";
                }
            }
            else
            {
                std::cout << "[-] Disabling speed hack\n";
                // Original bytes D9 81 20 01 00 00
                memory::patch((BYTE *)(speedHookAddress), (BYTE *)"\xD9\x81\x20\x01\x00\x00", speedHookLength);
                //*(float *)memory::findAddr(procBase + 0x01900600, {0xAC, 0x3C, 0x438, 0x18, 0x3E0, 0x120}) = 200; // Default speed
            }
        }

        // Zero Damage
        if (GetAsyncKeyState(VK_NUMPAD7) & 1)
        {
            bDamage = !bDamage;

            if (bDamage)
            {
                std::cout << "[+] Enabling zero damage\n";
                memory::patch((BYTE*)(moduleBase + 0x51176), (BYTE*)"\x0F\x84\x9C\x00\x00\x00", 6);
            }
            else
            {
                std::cout << "[-] Disabling zero damage\n";
                memory::patch((BYTE*)(moduleBase + 0x51176), (BYTE*)"\x0F\x85\x9C\x00\x00\x00", 6);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD8) & 1)
        {
            //Chat function address in GameLogic.dll
            DWORD chatHookAddress = moduleBase + 0x551A0;
            int chatHookLength = 8;

            //address in real function to jump back to after our code
            chatJmpBackAddr = chatHookAddress + chatHookLength;
            std::cout << "[-] Hooking into chat function\n";
            //our code, in hooks.cpp
            if (memory::hook((void*)chatHookAddress, player_chat_injected, chatHookLength))
            {
                std::cout << "[+] Enabled chat hack\n";
            }
        }

        
        if (GetAsyncKeyState(VK_NUMPAD9) & 1) {
            //increase piston damage by 100
            bPistol = !bPistol;
            DWORD akSpread = moduleBase + 0x13A00;
            DWORD akCooldown = moduleBase + 0x139E0;
            DWORD fireCooldown = moduleBase + 0x5263A;
            int fireCooldownReduceAmount = 5;
            int akCooldownReduceAmount = 6;
            int spreadReduceAmount = 6;
            if (bPistol)
            {
                std::cout << "[+] Enabling increased pistol damage\n";
                memory::patch((BYTE*)(moduleBase + 0x13930), (BYTE*)"\xB8\xFF\x00\x00\x00", 5);
                memory::patch((BYTE*)(moduleBase + 0x139F0), (BYTE*)"\xB8\xFF\xFF\x00\x00", 5);
                
                
                //address in real function to jump back to after our code
                akSpreadReduce = akSpread + spreadReduceAmount;
                std::cout << "[-] Hooking into spread reduction method\n";
                spread = 0.0f;
                //our code, in hooks.cpp
                if (memory::hook((void*)akSpread, bulletSpread, spreadReduceAmount))
                {
                    std::cout << "[+] Enabled spread hack\n";
                }
                    
                ////address in real function to jump back to after our code
                //akCooldownReduce = akCooldown + akCooldownReduceAmount;
                //std::cout << "[-] Hooking into cooldown reduction method\n";
                //cooldown = 0.0f;
                ////our code, in hooks.cpp
                //if (memory::hook((void*)akCooldown, cooldownReduce, akCooldownReduceAmount))
                //{
                //    std::cout << "[+] Enabled spread hack\n";
                //}

                //address in real function to jump back to after our code
                fireCooldownReduce = fireCooldown + fireCooldownReduceAmount;
                std::cout << "[-] Hooking into cooldown reduction method\n";
                fcooldown = 0.0f;
                //our code, in hooks.cpp
                if (memory::hook((void*)fireCooldown, fcooldownReduce, fireCooldownReduceAmount))
                {
                    std::cout << "[+] Enabled spread hack\n";
                }
                


            }
            else
            {
                std::cout << "[-] Disabling zero damage\n";
                memory::patch((BYTE*)(moduleBase + 0x13930), (BYTE*)"\xB8\x0E\x00\x00\x00", 5);
                //memory::patch((BYTE*)(moduleBase + 0x1DD6), (BYTE*)"\x0F\x57\xC0\x00\x00", 5);
            }
        }
    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr);
        if (thread)
            CloseHandle(thread);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}