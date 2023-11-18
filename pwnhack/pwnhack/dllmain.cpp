// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include "memory.cpp"
#include "proc.cpp"
#include "functions.cpp"

DWORD WINAPI HackThread(HMODULE hModule)
{
	//temporary console for debugging
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "  [DELETE] to quit\n";
	std::cout << "  [Numpad 1] for infinite mana            [Toggle]\n";
	std::cout << "  [Numpad 2] for Infinite Ammo            [Toggle]\n";
	std::cout << "  [Numpad 3] for infinite coins/items     [Toggle]\n";
	std::cout << "  [Numpad 4] for Teleport to Fire Spell\n";
	std::cout << "  [Numpad 5] for Teleport to Gun Shop\n";
	std::cout << "  [Numpad 6] for Speed Hack               [Toggle]\n";
	std::cout << "  [Numpad 7] for Zero Damage              [Toggle]\n";
	std::cout << "  [Numpad 8] cool, dmg, spread hack       [Toggle]\n";
	std::cout << "  [Numpad 9] View Player Coordinates      [Toggle]\n";

	DWORD procId = GetProcId(L"PwnAdventure3-Win32-Shipping.exe");
	uintptr_t procBase = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"GameLogic.dll");

	//Chat function address in GameLogic.dll
	DWORD chatHookAddress = moduleBase + 0x551A0;
	int chatHookLength = 8;

	//address in real function to jump back to after our code
	chatJmpBackAddr = chatHookAddress + chatHookLength;
	//our code, in functions.cpp
	if (hook((void*)chatHookAddress, player_chat_injected, chatHookLength))
	{
		std::cout << "[+] Enabled chat hack\n";
	}

	bool bAmmo = false, bSpeed = false, bMana = false, bItems = false, bDamage = false, bPistol = false;


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
				//nop 6 bytes which update mana
				nop((BYTE*)(moduleBase + 0x525C7), 6);
			}
			else
			{
				std::cout << "[-] Disabling infinite mana\n";
				//restore original bytes
				patch((BYTE*)(moduleBase + 0x525C7), (BYTE*)"\x89\x86\xbc\x00\x00\x00", 6);
			}
		}


		// Infinite ammo NOP
		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bAmmo = !bAmmo;

			if (bAmmo)
			{
				std::cout << "[+] Enabling infinite ammo\n";
				//nop 3 bytes which update mana
				nop((BYTE*)(moduleBase + 0x52396), 3);
			}
			else
			{
				std::cout << "[-] Disabling infinite ammo\n";
				// Look at IDA Hex View to find original hex values for patching
				patch((BYTE*)(moduleBase + 0x52396), (BYTE*)"\x89\x48\x1c", 3);
			}
		}

		// Infinite items NOP
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			bItems = !bItems;

			if (bItems)
			{
				std::cout << "[+] Enabling infinite coins/items\n";
				nop((BYTE*)(moduleBase + 0x52217), 3);
			}
			else
			{
				std::cout << "[-] Disabling infinite coins/items\n";
				patch((BYTE*)(moduleBase + 0x52217), (BYTE*)"\x89\x48\x18", 3);
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
				walkSpeed = 2000.0f;//custom speed
				//our code, in functions.cpp
				if (hook((void*)speedHookAddress, speedHook, speedHookLength))
				{
					std::cout << "[+] Enabled speed hack\n";
				}
			}
			else
			{
				std::cout << "[-] Disabling speed hack\n";
				//Original bytes
				patch((BYTE*)(speedHookAddress), (BYTE*)"\xD9\x81\x20\x01\x00\x00", speedHookLength);
			}
		}

		// Zero Damage
		if (GetAsyncKeyState(VK_NUMPAD7) & 1)
		{
			bDamage = !bDamage;


			if (bDamage)
			{
				std::cout << "[+] Enabling zero damage\n";
				// \x84 -> jnz patching with \x85 -> jz to skip the part which updates health after damage is taken
				patch((BYTE*)(moduleBase + 0x51176), (BYTE*)"\x0F\x84\x9C\x00\x00\x00", 6);
			}
			else
			{
				std::cout << "[-] Disabling zero damage\n";
				// restoring
				patch((BYTE*)(moduleBase + 0x51176), (BYTE*)"\x0F\x85\x9C\x00\x00\x00", 6);
			}
		}

		//op ak
		if (GetAsyncKeyState(VK_NUMPAD8) & 1) {
			bPistol = !bPistol;
			DWORD akSpreadHookAddress = moduleBase + 0x13A00;
			DWORD akCooldownHookAddress = moduleBase + 0x139E0;
			DWORD fCooldownHookAddress = moduleBase + 0x5263A;
			int fCooldownHookLength = 5;
			int akCooldownHookLength = 6;
			int akSpreadHookLength = 6;
			if (bPistol)
			{
				std::cout << "[+] Enabling increased ak damage\n";
				//patch((BYTE*)(moduleBase + 0x13930), (BYTE*)"\xB8\xFF\x00\x00\x00", 5);
				patch((BYTE*)(moduleBase + 0x139F0), (BYTE*)"\xB8\xFF\xFF\xFF\xFF", 5);


				//address in real function to jump back to after our code
				akSpreadJmpBackAddr = akSpreadHookAddress + akSpreadHookLength;
				std::cout << "[-] Hooking into ak spread method\n";
				spread = 0.0f;
				//our code, in functions.cpp
				if (hook((void*)akSpreadHookAddress, akSpread, akSpreadHookLength))
				{
					std::cout << "[+] Enabled ak spread hack\n";
				}


				//address in real function to jump back to after our code
				akCooldownJmpBackAddr = akCooldownHookAddress + akCooldownHookLength;
				std::cout << "[-] Hooking into ak cooldown method\n";
				cooldown = 0.0f;
				//our code, in functions.cpp
				if (hook((void*)akCooldownHookAddress, akCooldown, akCooldownHookLength))
				{
					std::cout << "[+] Enabled ak cooldown hack\n";
				}

				//address in real function to jump back to after our code
				fCooldownJmpBackAddr = fCooldownHookAddress + fCooldownHookLength;
				std::cout << "[-] Hooking into item cooldown method\n";
				fcooldown = 0.0f;
				//our code, in functions.cpp
				if (hook((void*)fCooldownHookAddress, itemCooldown, fCooldownHookLength))
				{
					std::cout << "[+] Enabled item cooldown hack\n";
				}

			}
			else
			{
				//disable all
				std::cout << "[-] Disabling hacks\n";
				patch((BYTE*)(moduleBase + 0x139F0), (BYTE*)"\xB8\x0B\x00\x00\x00", 5);
				patch((BYTE*)(moduleBase + 0x13A00), (BYTE*)"\xD9\x05\x20\x8B\x07\x10", akSpreadHookLength);
				patch((BYTE*)(moduleBase + 0x139E0), (BYTE*)"\xD9\x05\xF0\x8A\x07\x10", akCooldownHookLength);
				patch((BYTE*)(moduleBase + 0x5263A), (BYTE*)"\xF3\x0F\x10\x45\x0C", fCooldownHookLength);
			}
		}

		//player coordinates
		if (GetAsyncKeyState(VK_NUMPAD9) & 1) {
			float x = *(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x90 });
			float y = *(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x94 });
			float z = *(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x98 });

			std::cout << "x: " << x << "\n";
			std::cout << "y: " << y << "\n";
			std::cout << "z: " << z << "\n";
		}
	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

//TO DO:
//one tap
//fly
//spawn
//add items
//call other functions
//relative teleport
//more teleport locations
//minigame
//air jump

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