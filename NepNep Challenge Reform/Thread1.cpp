#include <Windows.h>
#include <cstdio>
#include <cstdint>
#include "Common.h"
#include <thread>

#define MX (((z >> 5) ^ (y << 2)) + ((y >> 3) ^ (z << 4))) ^ ((sum ^ y) + (key[(p & 3) ^ e] ^ z))
#define DELTA 0x9e3779b9

extern unsigned char input[SIZE + 1];
extern unsigned char output[SIZE + 1];
extern char switchState;
extern char terminateTimes;
extern Data data;
extern void AntiDebug1();
extern void AntiDebug2();
extern void AntiDebug3();
extern void getData();
extern int aesEncrypt(const uint8_t* key, uint32_t keyLen, const uint8_t* pt, uint8_t* ct, uint32_t len);
unsigned int* GenerateTeaKey();
unsigned char* GenerateAESKey();
static unsigned int* xxtea_uint_encrypt(unsigned int* data, size_t len, unsigned int* key);

void Thread1() {
	/*static char cot = 0;
	cot++;
	printf("TH1 %d, ", cot);*/
	while (switchState == OFF) {
		std::chrono::milliseconds dura(100);
		std::this_thread::sleep_for(dura);
	}
	switchState = OFF;
	AntiDebug3();
	//getData();
	auto keyTea = GenerateTeaKey();
	xxtea_uint_encrypt((unsigned int*)input, SIZE / 4, keyTea);
	//for (int i = 0; i < SIZE; i++)
	//	printf("%hx", input[i]);
	//printf("\n");
	for (int i = 0; i < SIZE - 1; i++) {
		if (input[i] == input[i + 1])
			data.cot_same += 1;
	}
	AntiDebug2();
	//getData();
	auto keyAES = GenerateAESKey();
	//for (int i = 0; i < SIZE; i++)
	//	printf("%hx", input[i]);
	//printf("\n");
	//printf("AESkey: ");
	//for (int i = 0; i < 16; i++)
	//	printf("%hx", keyAES[i]);
	//printf("\n");
	aesEncrypt((const unsigned char*)keyAES, 16, (const unsigned char*)input, (unsigned char*)output, SIZE);
	AntiDebug3();
	return;
}

unsigned int* GenerateTeaKey() {
	unsigned int* key = new unsigned int[4];
	key[0] = data.cot_Anti1 * 0x19;
	key[1] = data.cot_Anti2 * 0x28;
	key[2] = data.cot_Anti3 * 0x37;
	key[3] = data.cot_Anti1 + data.cot_Anti2 + data.cot_Anti3;
	//for (int i = 0; i < 4; i++)
	//	printf("TEAkey: %hx\n", key[i]);
	return key;
}

unsigned char* GenerateAESKey() {
	unsigned char* key = new unsigned char[16];
	key[0] = data.cot_Anti1 * 0x19;
	key[1] = data.cot_Anti2 * 0x28;
	key[2] = data.cot_Anti3 * 0x37;
	key[3] = data.cot_Anti1 + data.cot_Anti2 + data.cot_Anti3 + data.cot_same;
	key[4] = data.cot_Anti1 * 0x19;
	key[5] = data.cot_Anti2 * 0x28;
	key[6] = data.cot_Anti3 * 0x37;
	key[7] = data.cot_Anti1 + data.cot_Anti2 + data.cot_Anti3 - data.cot_same;
	key[8] = data.cot_Anti1 * 0x46;
	key[9] = data.cot_Anti2 * 0x55;
	key[10] = data.cot_Anti3 * 0x64;
	key[11] = data.cot_Anti1 + data.cot_Anti2 + data.cot_Anti3 + data.cot_same;
	key[12] = data.cot_Anti1 * 0x46;
	key[13] = data.cot_Anti2 * 0x55;
	key[14] = data.cot_Anti3 * 0x64;
	key[15] = data.cot_Anti1 + data.cot_Anti2 + data.cot_Anti3 - data.cot_same;
	return key;
}

static unsigned int* xxtea_uint_encrypt(unsigned int* data, size_t len, unsigned int* key)
{
	unsigned int n = (unsigned int)len - 1;
	// 6 和 52 是怎么来的？
	unsigned int z = data[n], y, p, q = 6 + 52 / (n + 1), sum = 0, e;
	if (n < 1)
		return data;

	while (0 < q--)
	{
		sum += DELTA;        // 根据sum 计算得出0~3中的某一个数值, 用于MX中与p共同作用选择key数组中某个秘钥值
		e = sum >> 2 & 3;
		// 遍历每个待加密的数据
		for (p = 0; p < n; p++)
		{
			// z的初值为data[len - 1]，即将数据数组当做是环形队列来处理的，首尾相连，当加密data[0]时，需要用到data[len - 1]，data[0]，data[0 + 1]，以及MX计算返回的的一个加密值，加密值与data[0]相加后达到加密的效果
			y = data[p + 1];
			z = data[p] += MX;
		}
		// 当加密data[len-1]时，需要用到data[len - 2]，data[len-1]，data[0]，以及MX计算返回的的一个加密值，加密值与data[len-1]相加后达到加密的效果
		y = data[0];
		z = data[n] += MX;
	}
	return data;
}