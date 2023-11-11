#include "pch.h"
#include "memory.h"

//VirtualProtect->Enable or disable read, write, and execute permissions in specified memory region
//patch overwritten assembly with original bytes(most of the time), usually to disable a hack
void memory::patch(BYTE *dst, BYTE *src, unsigned int size)
{
    DWORD oldProtect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(dst, src, size);
    VirtualProtect(dst, size, oldProtect, &oldProtect);
}

//nop specified no. of bytes
void memory::nop(BYTE *dst, unsigned int size)
{
    DWORD oldProtect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memset(dst, 0x90, size);
    VirtualProtect(dst, size, oldProtect, &oldProtect);
}

//to execute your own code
bool memory::hook(void *toHook, void *ourFunct, int len)
{

    //4 bytes are always overwritten
    if (len < 5)
    {
        return false;
    }

    DWORD curProtection;
    VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

    //nop bytes eequal to len
    memset(toHook, 0x90, len);

    //to determine relative jump address
    DWORD relativeAddress = ((DWORD)ourFunct - (DWORD)toHook) - 5;

    //relative jump to redirect flow to our function
    *(BYTE *)toHook = 0xE9;
    *(DWORD *)((DWORD)toHook + 1) = relativeAddress;

    DWORD temp;
    VirtualProtect(toHook, len, curProtection, &temp);

    return true;
}


//add offsets to address
uintptr_t memory::findAddr(uintptr_t ptr, std::vector<unsigned int> offsets)
{
    uintptr_t addr = ptr;

    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        addr = *(uintptr_t *)addr;
        addr += offsets[i];
    }
    return addr;
}
