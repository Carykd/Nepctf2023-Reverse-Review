#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Windows.h>
#include "Common.h"
#include <iostream>
#include <thread>

extern inline void AntiDebug1();
extern inline void AntiDebug2();
extern inline void AntiDebug3();
extern char switchState;
extern void PureAnti();
extern void Thread1();
extern void Thread2();
extern void Terminate();
void getData();

unsigned char input[SIZE + 1] = { 0 };
unsigned char output[SIZE + 1] = { 0 };
extern char terminateTimes;
Data data;

//void Crc32Test();
int main() {
	//Crc32Test();
	printf("> ");
	gets_s((char*)input, SIZE + 1);
	if (strncmp((const char*)input, "NepCTF{", 7) || (input[SIZE - 1] != '}')) {			// flag format check
		printf("Flag format: NepCTF{%%s}\n");
		exit(0);
	}
	AntiDebug1();															// check debugger
	AntiDebug3();
	auto pureAnti = std::thread(PureAnti);
	pureAnti.detach();
	auto terminate = std::thread(Terminate);
	terminate.detach();
	std::chrono::milliseconds dura(100);
	std::this_thread::sleep_for(dura);
	auto thread1 = std::thread(Thread1);
	switchState = ON;
	thread1.join();
	auto thread2 = std::thread(Thread2);
	switchState = ON;
	thread2.join();
	return 0;
}

//void Crc32Test()
//{
//	char* buffer = (char*)GetModuleHandleA(0);
//	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)buffer;
//	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(pDosHeader->e_lfanew + buffer);
//	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);
//	for (int i = 0; i < pNtHeader->FileHeader.NumberOfSections - 1; i++)
//	{
//		//if (pSectionHeader->Characteristics / 0x10000000 == 6)
//		//{
//		std::cout << pSectionHeader->Name << std::endl;
//		//auto CrcNum = make_crc((unsigned char*)(pSectionHeader->VirtualAddress + buffer), pSectionHeader->Misc.VirtualSize);
//		//std::cout << CrcNum << std::endl;
//		//}
//		std::cout << std::endl;
//		pSectionHeader++;
//	}
//}

void getData() {
	printf("cot_Anti1: %d\n", data.cot_Anti1);
	printf("cot_Anti2: %d\n", data.cot_Anti2);
	printf("cot_Anti3: %d\n", data.cot_Anti3);
	printf("cot_same: %d\n", data.cot_same);
	printf("\n");
}