#include <iostream>
#include <Windows.h>
#include <cstdio>
#include "Common.h"
#include <thread>

extern char switchState;
extern Data data;
extern unsigned char output[SIZE + 1];
extern void AntiDebug1();
extern void AntiDebug2();
extern void AntiDebug3();
extern bool exceptionAntiDebug(LPEXCEPTION_POINTERS p_exinfo);

char li[SIZE] = { 0 };

unsigned char check[SIZE] = { 0xf4, 0x9c, 0xdd, 0x41, 0x3, 0xdd, 0x5a, 0x13, 0x2e, 0x55, 0x97, 0x9e, 0xff, 0xd5, 0x8, 0xd9, 0xf6, 0xd1, 0x9, 0x8c, 0x68, 0x9e, 0x92, 0xff, 0x75, 0xf, 0x80, 0x95, 0x4b, 0x16, 0xb9, 0xc6, 0x7f, 0x54, 0x2e, 0x20, 0x35, 0xfc, 0x1b, 0x46, 0x14, 0xaa, 0xda, 0x5e, 0x4f, 0xbd, 0x59, 0x71 };

void Thread2() {
	/*static char cot = 0;
	cot++;
	printf("TH2 %d, ", cot);*/
	__try {
		while (switchState == OFF) {
			std::chrono::milliseconds dura(100);
			std::this_thread::sleep_for(dura);
		}
		switchState = OFF;
		for (int i = 0; i < SIZE; i++) {
			if (output[i] != check[i])
				throw(EXCEPTION_ACCESS_VIOLATION);
		}
		printf("\n");
	}
	__except (exceptionAntiDebug(GetExceptionInformation())) {
		HANDLE hprocess = GetCurrentProcess();
		printf("Need password to print your flag...\n");
		ReadFile(GetStdHandle(STD_INPUT_HANDLE), li, SIZE, NULL, NULL);
		printf("This may take about 10 seconds...\nPlease wait patiently...\n");
		std::chrono::milliseconds dura(10000);
		std::this_thread::sleep_for(dura);
		for (int i = 0; i < SIZE; i++) {
			li[i] += 1;
		}
		printf("Here is your flag: \n");
		std::cout << li << std::endl;
		return;
	}
	char val[] = { 87, 110, 117, 47, 36, 124, 105, 114, 40, 104, 120, 110, 44, 127, 107, 110, 124, 125, 107, 51, 117, 123, 54, 114, 96, 109, 104, 122, 115, 111, 122, 118, 78, 64, 80, 90, 4, 87, 67, 81, 77, 91, 89, 78, 94, 12 };
	for (int i = 0; i < 46; i++) {
		val[i] ^= i;
	}
	printf("%s", val);
	return;
}