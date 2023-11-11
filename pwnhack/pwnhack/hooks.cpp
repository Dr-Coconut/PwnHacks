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