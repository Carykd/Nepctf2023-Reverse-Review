#pragma once
#define SIZE 48
#define ON 1
#define OFF 0

struct Data {
	char cot_Anti1 = 0;	// IsDebuggerPresent
	char cot_Anti2 = 0;	// exceptionAntiDebug
	char cot_Anti3 = 0;	// CRC32
	char cot_same = 0;	// Same letter besides
};