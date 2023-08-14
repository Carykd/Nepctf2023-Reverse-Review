#include <Windows.h>
#include "Common.h"
#include <cstdio>
#include <cstdlib>
#include <thread>

char switchState = OFF;
char terminateState = OFF;
char terminateTimes = 0;

unsigned int crc32_table[256];
static unsigned char* string;
static unsigned int size;
extern unsigned char input[SIZE+1];
extern Data data;
void CRC(unsigned int crc1);

extern void AntiDebug1() {
	/*static char cot = 0;
	cot++;
	printf("Anti1 %d, ", cot);*/
	if (IsDebuggerPresent()) {
		input[10] ^= 3;
		input[7] ^= 2;
		input[9] += 4;
		data.cot_Anti1 += 1;
	}
	data.cot_Anti1 += 1;
}

inline bool exceptionAntiDebug(LPEXCEPTION_POINTERS p_exinfo) {
	PCONTEXT context = p_exinfo->ContextRecord;
	PEXCEPTION_RECORD exception = p_exinfo->ExceptionRecord;
	context->Dr0 = 0;
	context->Dr1 = 0;
	context->Dr2 = 0;
	context->Dr3 = 0;
	return 1;
}

extern inline void AntiDebug2() {
	/*static char cot = 0;
	cot++;
	printf("Anti2 %d, ", cot);*/
	__try {
		//HANDLE hprocess = GetCurrentProcess();
		//ReadFile(GetStdHandle(STD_INPUT_HANDLE), input, 17, NULL, NULL);
		__asm {
			mov eax, 0
			div eax
		}
	}
	__except (exceptionAntiDebug(GetExceptionInformation())) {
		DWORD e = GetExceptionCode();
		if (e == EXCEPTION_INT_DIVIDE_BY_ZERO) {
			for (auto& i : input)
				i = ~i;
		}
		else if (e == EXCEPTION_BREAKPOINT) {
			data.cot_Anti2 += 2;
		}
	}
	data.cot_Anti2 += 1;
}

int make_crc32_table()
{
	unsigned int c;
	int i = 0;
	int bit = 0;
	for (i = 0; i < 256; i++) {
		c = (unsigned int)i;
		for (bit = 0; bit < 8; bit++) {
			if (c & 1)
				c = (c >> 1) ^ (0xEDB88320);
			else
				c = c >> 1;
		}
		crc32_table[i] = c;
	}
	return 1;
}

inline void checkCRC()
{
	static int cot = 0;
	/*printf("checkCRC: %d, ", ++cot);*/
	int local_cot = cot;
	unsigned int crc1 = 0xFFFFFFFF;
	unsigned int crc2 = 0xFFFFFFFF;
	unsigned char* p_string = string;
	make_crc32_table();
	auto temp = size;
	int times = 0;
	while (temp--)
		crc1 = (crc1 >> 8) ^ (crc32_table[(crc1 ^ *p_string++) & 0xff]);
	p_string = string;
	temp = size;
	while (temp--)
		crc2 = (crc2 >> 8) ^ (crc32_table[(crc2 ^ *p_string++) & 0xff]);
	times++;
	if (crc2 != crc1 && times == 1) {
		printf("checkCRC failed: %d, ", local_cot);
		data.cot_Anti3 += 3;
	}
	/*else
		printf("checkCRC passed: %d, ", local_cot);*/
	crc2 = 0xFFFFFFFF;
	auto crc = std::thread(CRC, crc1);
	crc.detach();
}

extern inline void AntiDebug3()
{
	/*static char cot = 0;
	cot++;
	printf("Anti3 %d, ", cot);*/
	char* buffer = (char*)GetModuleHandleA(0);
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)buffer;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(pDosHeader->e_lfanew + buffer);
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);
	for (int i = 0; i < pNtHeader->FileHeader.NumberOfSections - 1; i++) {
		if (pSectionHeader->Characteristics / 0x10000000 == 6) {	// getCodeSectionHeader
			//std::cout << pSectionHeader->Name << std::endl;
			data.cot_Anti3 += 1;
			string = (unsigned char*)(pSectionHeader->VirtualAddress + buffer);
			size = pSectionHeader->Misc.VirtualSize;
			checkCRC();
			//std::cout << CrcNum << std::endl;
		}
		pSectionHeader++;
	}
}

void CRC(unsigned int crc1) {
	static int cot = 0;
	/*printf("CRC: %d, ", ++cot);*/
	int local_cot = cot;
	unsigned int crc2 = 0xFFFFFFFF;
	int times = 1;
	do {
		auto p_string = string;
		auto temp = size;
		while (temp--)
			crc2 = (crc2 >> 8) ^ (crc32_table[(crc2 ^ *p_string++) & 0xff]);
		times++;
		if (crc2 != crc1) {
			printf("CRC failed: %d, ", local_cot);
			exit(0);
		}
		//else
		//	printf("CRC passed: %d, ", local_cot);
		crc2 = 0xFFFFFFFF;
		Sleep(200);
	} while (true);
}

void PureAnti() {
	/*static char cot = 0;
	cot++;
	printf("Pure %d, ", cot);*/
	do {
		__try {
			//HANDLE hprocess = GetCurrentProcess();
			//ReadFile(GetStdHandle(STD_INPUT_HANDLE), input, 17, NULL, NULL);
			__asm {
				mov eax, 0
				div eax
			}
		}
		__except (exceptionAntiDebug(GetExceptionInformation())) {
			DWORD e = GetExceptionCode();
			if (e == EXCEPTION_BREAKPOINT) {
				printf("Breakpoint checked!");
				terminateState = ON;
			}
		}
		if (terminateState == ON)
			terminateTimes++;
		std::chrono::milliseconds dura(300);
		std::this_thread::sleep_for(dura);
	} while (true);
}

void Terminate() {
	while (terminateTimes == 0) {
		std::chrono::milliseconds dura(100);
		std::this_thread::sleep_for(dura);
	}
	while (terminateTimes <= 50) {
		std::chrono::milliseconds dura(300);
		std::this_thread::sleep_for(dura);
	}
	printf("Big brother is watching you");
	exit(0);
}