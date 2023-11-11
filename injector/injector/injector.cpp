#include <Windows.h>
#include <TlHelp32.h>

//function to get Process ID of the target process
//const wchar_t* -> Wide Character String for different encoding
uintptr_t getProcId(const wchar_t* procName)
{
    uintptr_t procId = 0;
    //snapshot of running processes
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        //individual process
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry))
        {
            //loop to check for match
            do
            {
                if (!_wcsicmp(procEntry.szExeFile, procName))
                {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);
    return procId;
}

int main() {
    //name of DLL
    const wchar_t* dllName = L"pwnhack.dll"; 
    wchar_t dllPath[MAX_PATH];

    //current working directory
    GetCurrentDirectory(MAX_PATH, dllPath);

    //to get Path
    wcscat_s(dllPath, L"\\");
    wcscat_s(dllPath, dllName);

    //process ID
    DWORD procId = getProcId(L"PwnAdventure3-Win32-Shipping.exe");

    if (procId != 0) {
        //open a handle to the target process with permissions
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);

        if (hProcess) {
            //allocate memory in process
            LPVOID remoteString = VirtualAllocEx(hProcess, 0, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            if (remoteString) {
                //write DLL path, *2 because wide characters are 2 bytes each
                WriteProcessMemory(hProcess, remoteString, dllPath, wcslen(dllPath) * 2, NULL);

                //create remote thread, essentially to load DLL 
                HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, remoteString, 0, NULL);
                if (hThread) {
                    WaitForSingleObject(hThread, INFINITE);
                    CloseHandle(hThread);
                }

                VirtualFreeEx(hProcess, remoteString, 0, MEM_RELEASE);
            }

            CloseHandle(hProcess);
        }
    }

    return 0;
}