#include "proc.h"

DWORD getProcId(const wchar_t* procName) {

	DWORD procId = 0;
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 procEntry32;
		procEntry32.dwSize = sizeof(procEntry32);

		if (Process32Next(hProcSnap, &procEntry32)) {
			do {
				if (!wcscmp(procName, procEntry32.szExeFile)) {
					procId = procEntry32.th32ProcessID;
					break;

				}
			} while (Process32Next(hProcSnap, &procEntry32));
		}

	}
	CloseHandle(hProcSnap);
	return procId;
}

uintptr_t getModuleBaseAddr(DWORD procId, const wchar_t* modName) {

	uintptr_t modAddr = 0;
	HANDLE hModSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

	if (hModSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32Next(hModSnap, &modEntry)) {

			do {
				if (!wcscmp(modEntry.szModule, modName)) {
					modAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hModSnap, &modEntry));
		}
	}
	CloseHandle(hModSnap);
	return modAddr;
}

uintptr_t findMAAddyAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}
