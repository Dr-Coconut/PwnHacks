#pragma once
#include <Windows.h>
#include <vector>

namespace memory
{
	void nop(BYTE* dst, unsigned int size);
	void patch(BYTE* dst, BYTE* src, unsigned int size);	
	bool hook(void* toHook, void* ourFunct, int len);
	uintptr_t findAddr(uintptr_t ptr, std::vector<unsigned int> offsets);	
}