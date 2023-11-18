#include "pch.h"
#include "libGameLogic.h"
#include <string>
#include <sstream>

DWORD procId = GetProcId(L"PwnAdventure3-Win32-Shipping.exe");
uintptr_t procBase = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");
uintptr_t moduleBase = GetModuleBaseAddress(procId, L"GameLogic.dll");

DWORD speedJmpBackAddr;
float walkSpeed; // Our custom walk speed
void __declspec(naked) speedHook()
{
	__asm {
		fld walkSpeed
		jmp[speedJmpBackAddr] // Jump back to original code
	}
}

DWORD akSpreadJmpBackAddr;
float spread;
void __declspec(naked) akSpread()
{
	__asm {
		fld spread
		jmp[akSpreadJmpBackAddr] // Jump back to original code
	}
}

DWORD akCooldownJmpBackAddr;
float cooldown;
void __declspec(naked) akCooldown()
{
	__asm {
		fld cooldown
		jmp[akCooldownJmpBackAddr] // Jump back to original code
	}
}

DWORD fCooldownJmpBackAddr;
float fcooldown;
void __declspec(naked) itemCooldown()
{
	__asm {
		movss xmm0, fcooldown
		jmp[fCooldownJmpBackAddr] // Jump back to original code
	}
}

void on_player_chat(char* msg, char* player);
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

//values from cheat engine
void teleport(uintptr_t procBase, float x, float y, float z)
{
	*(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x90 }) = x;
	*(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x94 }) = y;
	*(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x98 }) = z;
}

void addCoins(int amount) {
	*(int*)findAddr(moduleBase + 0x97D7C, { 0x1C, 0x6C, 0x4C, 0x0, 0X18 }) += amount;
	// adding offsets to the base game dll address to find the address of the coins variable and adding the amount to it
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
	processInput(str);
}