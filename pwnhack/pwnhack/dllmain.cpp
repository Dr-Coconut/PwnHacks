// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include "memory.cpp"
#include "proc.cpp"
#include "functions.cpp"

DWORD WINAPI HackThread(HMODULE hModule)
{
	//console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "                                                                                              \n";
	std::cout << "                                                                                              \n";
	std::cout << "    ##   ##  #####   ###### ##   ##     ###    ###  #####  ###    ## ##    ##  #####  ##      \n";
	std::cout << "    ##   ## ##   ## ##      ##  ##      ####  #### ##   ## #####  ## ##    ## ##   ## ##      \n";
	std::cout << "    ####### ####### ##      ######      ## #### ## ####### ## ##  ## ##    ## ####### ##      \n";
	std::cout << "    ##   ## ##   ## ##      ##  ##      ##  ##  ## ##   ## ##  ##### ##    ## ##   ## ##      \n";
	std::cout << "    ##   ## ##   ##  ###### ##   ##     ##      ## ##   ## ##   ####  ######  ##   ## ######  \n";
	std::cout << "                                                                                              \n";
	std::cout << "                                                                                              \n";
	std::cout << "      'infinite mana'       : Infinite Mana                 [ON/OFF]\n";
	std::cout << "      'infinite ammo'       : Infinite Ammo                 [ON/OFF]\n";
	std::cout << "      'infinite coins'      : Infinite Coins/Items          [ON/OFF]\n";
	std::cout << "      'infinite jump'       : Jump in the air               [ON/OFF]\n";
	std::cout << "      'battle               : SECRET!!!                     [ON/OFF]\n";
	std::cout << "      'speed'               : Speed Hack                    [ON/OFF]\n";
	std::cout << "      'zero dmg'            : Zero Damage(infinite health)  [ON/OFF]\n";
	std::cout << "      'zero cd'             : Zero Cooldown(spells)         [ON/OFF]\n";
	std::cout << "      'op ak'               : Overpowered AKRifle           [ON/OFF]\n";
	std::cout << "      'spawn [name]'        : Spawn Actor                                       \n";
	std::cout << "                              Available Actors:                                 \n";
	std::cout << "                             - 'bear'            : Bear                         \n";
	std::cout << "                             - 'giantrat'        : Giant Rat                    \n";
	std::cout << "                             - 'justintolerable' : Justin Tolerable             \n";
	std::cout << "                             - 'magmarok'        : Magmarok                     \n";
	std::cout << "                             - 'farmer'          : Farmer                       \n";
	std::cout << "                             - 'michaelangelo'   : Michael Angelo               \n";
	std::cout << "                             - 'cavespider'      : Cave Spider                  \n";
	std::cout << "                             - 'icespiderqueen'  : Ice Spider Queen             \n";
	std::cout << "                             - 'angrybear'       : Angry Bear                   \n";
	std::cout << "                             - 'madcow'          : Mad Cow                      \n";
	std::cout << "                             - 'cowking'         : Cow King                     \n";
	std::cout << "      'sethealth [int]'     : Set Health                                        \n";
	std::cout << "      'setmana [int]'       : Set Mana                                          \n";
	std::cout << "      'addcoins [int]'      : Add Coins                                         \n";
	std::cout << "      'tp [x] [y] [z]'      : Telport (x,y,z->float)                            \n";
	std::cout << "      'rtp [x] [y] [z]'     : Relative Telport (x,y,z->float)                   \n";
	std::cout << "      'tp [name]'           : Teleport to location                              \n";
	std::cout << "                              Available Locations:                              \n";
	std::cout << "                             - 'PirateBay'      : Pirate Bay                    \n";
	std::cout << "                             - 'GoldFarm'       : Gold Farm                     \n";
	std::cout << "                             - 'BallmerPeak'    : Ballmer Peak                  \n";
	std::cout << "                             - 'UnbearableWoods' : Unbearable Woods             \n";
	std::cout << "                             - 'Sewer'          : Sewer                         \n";
	std::cout << "                             - 'LostCave'       : Lost Cave                     \n";
	std::cout << "                             - 'MoltenCave'     : Molten Cave                   \n";
	std::cout << "                             - 'FortBlox'      : Fort Blox                      \n";
	std::cout << "                             - 'FireSpell'     : Fire Spell                     \n";
	std::cout << "                             - 'GunShop'       : Gun Shop                       \n";
	std::cout << "      'item [name] [int]'   : Give Item with Amount                             \n";
	std::cout << "                              Available Items:                                  \n";
	std::cout << "                             - 'GreatBallsOfFire'  : Great Balls of Fire        \n";
	std::cout << "                             - 'HandCannon'        : Hand Cannon                \n";
	std::cout << "                             - 'ROPChainGun'       : ROP Chain Gun              \n";
	std::cout << "                             - 'GoldMaster'        : Gold Master                \n";
	std::cout << "                             - 'RemoteExploit'     : Remote Exploit             \n";
	std::cout << "                             - 'CharStar'          : Char Star                  \n";
	std::cout << "                             - 'HeapSpray'         : Heap Spray                 \n";	
	std::cout << "                             - 'PistolAmmo'        : Pistol Ammo                \n";
	std::cout << "                             - 'ShotgunAmmo'       : Shotgun Ammo               \n";
	std::cout << "                             - 'SniperAmmo'        : Sniper Ammo                \n";
	std::cout << "                             - 'RevolverAmmo'      : Revolver Ammo              \n";
	std::cout << "                             - 'RifleAmmo'         : Rifle Ammo                 \n";
	std::cout << "                             - 'ZeroCool'          : Zero Cool                  \n";
	std::cout << "                             - 'TheCowboyCoder'    : The Cowboy Coder           \n";
	std::cout << "                             - 'HolyHandGrenade'   : Holy Hand Grenade          \n";
	std::cout << "                             - 'BearSkin'          : Bear Skin                  \n";
	std::cout << "                             - 'StaticLink'        : Static Link                \n";
	std::cout << "                                                                                \n";
	 
	DWORD procId = GetProcId(L"PwnAdventure3-Win32-Shipping.exe");
	uintptr_t procBase = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"GameLogic.dll");

	//Chat function address in GameLogic.dll
	DWORD chatHookAddress = moduleBase + 0x551A0;
	//skipping 8 bytes from 'chatHookAddress'
	int chatHookLength = 8;

	//address in real function to jump back to after our code
	chatJmpBackAddr = chatHookAddress + chatHookLength;
	//our code, in functions.cpp
	if (hook((void*)chatHookAddress, player_chat, chatHookLength))
	{
		std::cout << "[+] Enabled chat hack\n";
	}

	while (true)
	{
		// exit
		if (GetAsyncKeyState(VK_DELETE) & 1)
			break;
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