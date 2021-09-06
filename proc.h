#pragma once
#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include <iostream>

DWORD getProcId(const wchar_t* procName);

uintptr_t getModuleBaseAddr(DWORD procID, const wchar_t* modName);

uintptr_t findMAAddyAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);
