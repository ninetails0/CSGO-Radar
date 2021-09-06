#include <windows.h>
#include <iostream>
#include "proc.h"

template <class val>
val readMem(uintptr_t addr, HANDLE process);

template <class val>
val writeMem(uintptr_t addr, HANDLE process, val x);


struct offsets {
	
	uintptr_t entityList = 0x4DA31EC; //need to update offset
	uintptr_t isSpotted = 0x93D; //need to update offset
} offset;

struct variables {

	uintptr_t gameModule;
} val;

template <class val>
val readMem(uintptr_t addr, HANDLE process) {
	val x;
	ReadProcessMemory(process, (BYTE*)addr, &x, sizeof(x), NULL);
	return x;
}

template <class val>
val writeMem(uintptr_t addr, HANDLE process, val x) {
	WriteProcessMemory(process, (BYTE*)addr, &x, sizeof(x), NULL);
	return 0;
}

int main() {
	// get the entity-> gameModule + entityList + entityID * 0x10;
	DWORD procId = getProcId(L"csgo.exe");

	HANDLE hProc = NULL;
	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
	if (hProc == NULL) {
		std::cout << "Process not found" << std::endl;
		return 0;
	}

	val.gameModule = getModuleBaseAddr(procId, L"client.dll");
	bool b1 = true;

	while (true) {
		for (short int i = 0; i < 64; i++) {
			DWORD entity = readMem<DWORD>(val.gameModule + offset.entityList + i * 0x10, hProc);
			if (entity != NULL) {
				//std::cout << "entity found." << std::endl;
				//WriteProcessMemory(hProc, (BYTE*)entity + offset.isSpotted, &b1, sizeof(b1), NULL);
				writeMem<bool>(entity + offset.isSpotted, hProc, true);
			}
		}
		Sleep(1);
	}

	return 0;
}
