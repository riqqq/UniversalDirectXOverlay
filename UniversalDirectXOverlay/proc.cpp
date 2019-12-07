#include "proc.h"

DWORD GetProcId(const char* procName) {
	DWORD procId{ 0 }; //Set to 0 for error checking
	HANDLE hSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) }; //Get snapshot of all processes
	if (hSnap != INVALID_HANDLE_VALUE) { // If above didn't fail
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry); //According to documentation, you need to set it for it to work correctly

		if (Process32First(hSnap, &procEntry)) { //Grabs the first proccess in the snapshot and store it into procEntry

			do { //Loop through all processes
				if (!_stricmp(procEntry.szExeFile, procName)) { //Compare process file name to our given procName (string compare, case insensitive)
					procId = procEntry.th32ProcessID;
					break;
				}

			} while (Process32Next(hSnap, &procEntry)); //While there is still process left to loop
		}
	}
	CloseHandle(hSnap); //Close handle to the snapshot - prevents memory leaks
	return procId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName) { //uintptr will compile to x64 or x86, choose the one that game uses when compiling (top panel)
	uintptr_t modBaseAddr{ 0 };
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!_stricmp(modEntry.szModule, modName)) {
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) { //Takes a handle to a process and base ptr and vector which is going to be offsets
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); i++) {
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}