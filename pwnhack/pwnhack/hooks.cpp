#include "pch.h"
#include "libGameLogic.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
// This file contains the hooks that we want to overwrite


DWORD speedJmpBackAddr; // Address to jump back to after hooking
float walkSpeed; // Our custom walk speed
void __declspec(naked) speedHook()
{
	__asm {		
			fld walkSpeed // Load our custom walk speed
			jmp[speedJmpBackAddr] // Jump back to the line after the original fld operation
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

DWORD fireCooldownReduce;
float fcooldown;
void __declspec(naked) fcooldownReduce()
{
	__asm {
		movss xmm0, fcooldown
		jmp[fireCooldownReduce] // Jump back to original code
	}
}


DWORD procId = GetProcId(L"PwnAdventure3-Win32-Shipping.exe");
uintptr_t procBase = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");
uintptr_t moduleBase = GetModuleBaseAddress(procId, L"GameLogic.dll");

void teleport(uintptr_t procBase, float x, float y, float z);


void addCoins(int amount) {
	*(int*)memory::findAddr(moduleBase + 0x97D7C, { 0x1C, 0x6C, 0x4C, 0x0, 0X18 }) += amount; // adding offsets to the base game dll address to find the address of the coins variable and adding the amount to it
}


void processInput(const std::string& input) { //splitting the input into tokens/words
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

	if (!tokens.empty() && tokens[0] == "teleport") {
		if (tokens.size() == 4) {
			// extracting x, y, and z coordinates
			float x, y, z;
			std::istringstream(tokens[1]) >> x;
			std::istringstream(tokens[2]) >> y;
			std::istringstream(tokens[3]) >> z;

			// call the teleport function with the extracted coordinates
			teleport(procBase, x, y, z);
		}
		else {
			std::cout << "Incorrect number of coordinates provided after 'teleport'. Use format: teleport x y z." << std::endl;
		}
	}
	else {
		std::cout << "Input does not start with 'teleport'." << std::endl;
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