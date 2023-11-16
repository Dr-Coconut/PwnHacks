#include "pch.h"
#include "libGameLogic.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
// This file contains the hooks that we want to overwrite


DWORD speedJmpBackAddr;
float walkSpeed;
void __declspec(naked) speedHook()
{
	__asm {		
			fld walkSpeed
			jmp[speedJmpBackAddr] // Jump back to original code
	}
}


DWORD akSpreadReduce;
float spread;
void __declspec(naked) bulletSpread()
{
	__asm {		
			fld spread
			jmp[akSpreadReduce] // Jump back to original code
	}
}

DWORD akCooldownReduce;
float cooldown;
void __declspec(naked) cooldownReduce()
{
	__asm {
		fld cooldown
		jmp[akCooldownReduce] // Jump back to original code
	}
}


DWORD procId = GetProcId(L"PwnAdventure3-Win32-Shipping.exe");
uintptr_t procBase = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");
uintptr_t moduleBase = GetModuleBaseAddress(procId, L"GameLogic.dll");

void teleport(uintptr_t procBase, float x, float y, float z);


void addCoins(int amount) {
	*(int*)memory::findAddr(moduleBase + 0x97D7C, { 0x1C, 0x6C, 0x4C, 0x0, 0X18 }) += amount;
	//*(int*)memory::findAddr(moduleBase + 0x97D7C, { 0x1C, 0x6C, 0x4C, 0x8, 0X18 }) += amount;
	//*(int*)memory::findAddr(moduleBase + 0x97D7C, { 0x1C, 0x6C, 0x4C, 0x4, 0X18 }) += amount;
}


void processInput(const std::string& input) {
	std::istringstream iss(input);
	std::vector<std::string> tokens;
	std::string token;

	while (iss >> token) {
		tokens.push_back(token);
	}

	if (!tokens.empty() && tokens[0] == "fire") {
		teleport(procBase, -43644.0f, -56041.0f, 309.0f);
		//hkghkj
	}

	if (!tokens.empty() && tokens[0] == "coins") {
		if (tokens.size() > 1) {
			// extracting the value after 'coins'
			int amount;
			std::istringstream(tokens[1]) >> amount;

			// call function to add coins with the extracted amount
			addCoins(amount);
		}
		else {
			std::cout << "no value provided after 'coins'." << std::endl;
		}
	}
	else {
		std::cout << "input does not start with 'coins'." << std::endl;
	}


}


void on_player_chat(char* msg, char* player) {
	std::string str(msg);
	/*if (str == "fire") {
		teleport(procBase, -43644.0f, -56041.0f, 309.0f);
	}*/
	processInput(str);

	std::cout << "yo";
	std::cout << "proc addr" << std::hex << procBase << std::endl;
	std::cout << str << std::endl;
}

//void addCoins(amount) {
//	*(int*)memory::findAddr(moduleBase + 0x97D7C, { 0x1C, 0x6C, 0x4C, 0x0, 0X18 }) += amount;
//}

DWORD chatJmpBackAddr;
void __declspec(naked) player_chat_injected() {
	__asm {
		push ecx
		mov eax, [esp + 0x8]
		push eax
		call on_player_chat
		pop eax
		pop ecx
		push ebp
		mov ebp, esp
		and esp, 0x0fffffff8
		push 0xffffffff
		jmp[chatJmpBackAddr]
	}
}