#include "pch.h"
#include "libGameLogic.h"
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

void teleport(uintptr_t procBase, float x, float y, float z);
uintptr_t procBase = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");
void on_player_chat(char* msg, char* player) {
	std::string str(msg);
	if (str == "fire") {
		teleport(procBase, -43644.0f, -56041.0f, 309.0f);
	}
	std::cout << "yo";
	std::cout << "proc addr" << std::hex << procBase << std::endl;
	std::cout << str << std::endl;
}
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