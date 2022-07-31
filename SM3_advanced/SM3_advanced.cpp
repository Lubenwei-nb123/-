#include "SM3_advanced.h"
#include<iostream>
#include<set>
#include<array>
#include<windows.h>
#include<thread>
#include<vector>
using namespace std;
class TimeCounter {
public:
	TimeCounter(void) {
		QueryPerformanceFrequency(&CPUClock);
	}
	double timeInterval;
private:
	LARGE_INTEGER startTime, endTime, CPUClock;

public:
	void start() {
		QueryPerformanceCounter(&startTime);
	}
	void end() {
		QueryPerformanceCounter(&endTime);
		timeInterval = 1e3 * ((double)endTime.QuadPart - (double)startTime.QuadPart) / (double)CPUClock.QuadPart;
		//ms
	}
};
static void _CF(unsigned char* ucpSrcMsg, unsigned int nHash[8]);
void rho_attack_8bits() {
	unsigned char M[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}, H[32]={0};
	set<unsigned char> hash_prefix;
	unsigned char tmp = 0;
	for (int i = 0; i < pow(2, 32); i++) {
		SM3_256(M, 6, H);
		tmp = H[0];
		if (i != 0) {
			if (hash_prefix.find(tmp) != hash_prefix.end()) {
				cout << "collide successfully with " << hash_prefix.size() << " prefixs!" << endl;
				return;
			}
		}
		hash_prefix.insert(tmp);
		for (int j = 0; j < 5; j++) {
			M[j]  = M[j] + 2;
		}
	}
}
void rho_attack_16bits() {
	unsigned char M[6] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06}, H[32]={0};
	set<array<unsigned char, 2>> hash_prefix;
	array<unsigned char, 2> tmp = { 0 };
	for (int i = 0; i < pow(2, 32); i++) {
		SM3_256(M, 6, H);
		for (int j = 0; j < 2; j++) {
			tmp[j] = H[j];
		}
		if (i != 0) {
			if (hash_prefix.find(tmp) != hash_prefix.end()) {
				cout << "collide successfully with " << hash_prefix.size() << " prefixs!" << endl; 
				return;
			}
		}
		hash_prefix.insert(tmp);
		for (int j = 0; j < 5; j++) {
			M[j] = M[j] + 2;
		}
	}
}
void rho_attack_128bits() {
	unsigned char M[6] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 }, H[32]={0};
	set<array<unsigned char, 16>> hash_prefix;
	array<unsigned char, 16> tmp = { 0 };
	for (int i = 0; i < pow(2, 32); i++) {
		SM3_256(M, 6, H);
		for (int j = 0; j < 16; j++) {
			tmp[j] = H[j];
		}
		if (i != 0) {
			if (hash_prefix.find(tmp) != hash_prefix.end()) {
				cout << "collide successfully with " << hash_prefix.size() << " prefixs!" << endl;
				return;
			}
		}
		hash_prefix.insert(tmp);
		for (int j = 0; j < 5; j++) {
			M[j] = M[j] + 2;
		}
	}
}
void rho_attack_256bits() {
	unsigned char M[6] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 }, H[32]={0};
	set<array<unsigned char, 32>> hash_prefix;
	array<unsigned char, 32> tmp = { 0 };
	for (int i = 0; i < pow(2, 32); i++) {
		SM3_256(M, 6, H);
		for (int j = 0; j < 32; j++) {
			tmp[j] = H[j];
		}
		if (i != 0) {
			if (hash_prefix.find(tmp) != hash_prefix.end()) {
				cout << "collide successfully with " << hash_prefix.size() << " prefixs!" << endl;
				return;
			}
		}
		hash_prefix.insert(tmp);
		for (int j = 0; j < 5; j++) {
			M[j] = M[j] + 2;
		}
	}
}
void rho_collision() {
	cout << "以下是rho方法攻击的演示" << endl;
	TimeCounter t1, t2, t3, t4;
	cout << "rho method for 8 bits" << endl;
	t1.start();
	rho_attack_8bits();
	t1.end();
	cout << "Time consumption：" << t1.timeInterval << " " << "ms" << endl;
	cout << "rho method for 16 bits" << endl;
	t2.start();
	rho_attack_16bits();
	t2.end();
	cout << "Time consumption：" << t2.timeInterval << " " << "ms" << endl;
	cout << "rho method for 128 bits" << endl;
	t3.start();
	rho_attack_128bits();
	t3.end();
	cout << "Time consumption：" << t3.timeInterval << " " << "ms" << endl;
	cout << "rho method for 256 bits" << endl;
	t4.start();
	rho_attack_256bits();
	t4.end();
	cout << "Time consumption：" << t3.timeInterval << " " << "ms" << endl;
}
void birthday_attack_nbits(int n) {
	TimeCounter t;
	t.start();
	int length = n / 8;
	unsigned char* M1 = new unsigned char [length], * M2 = new unsigned char [length];
	unsigned char H1[32], H2[32];
	srand((unsigned)time(NULL));
	for (int i = 0; i < pow(2, length * 8); i++) {
		for (int j = 0; j < length; j++) {
			M1[j] = rand() % (0xff + 1);
		}
		for (int j = 0; j < length; j++) {
			M2[j] = rand() % (0xff + 1);
		}
		SM3_256(M1, length, H1);
		SM3_256(M2, length, H2);
		int stop;
		for (stop = 0; stop < length; stop++) {
			if (H1[stop] != H2[stop]) {
				break;
			}
		}
		if (stop == length) {
			cout << "collide successfully!" << endl;
			break;
		}
	}
	t.end();
	cout << "Time consumption：" << t.timeInterval << " ms" << endl;
}
void birthday_attack() {
	cout << "以下是生日攻击" << endl;
	cout << "birthday attack for 8 bits：" << endl;
	birthday_attack_nbits(8);
	cout << "birthday attack for 16 bits：" << endl;
	birthday_attack_nbits(16);
	cout << "birthday attack for 64 bits：" << endl;
	birthday_attack_nbits(64);
	cout << "birthday attack for 128 bits：" << endl;
	birthday_attack_nbits(128);
	cout << "birthday attack for 256 bits：" << endl;
	birthday_attack_nbits(256);
}
void extension_attack() {
	cout << "以下是长度扩展攻击：" << endl;
	unsigned char H0[32] = { 0 };
	unsigned char H1[32] = { 0 };
	unsigned char H2[32] = { 0 };
	unsigned char str[] = { 'S','D','U','\0'};
	/*	
	"SDU"的哈希值：	
		0x62  0xa4  0x48  0x85
		0x19  0x60  0x86  0x90
		0x90  0x7e  0xad  0xf9
		0x34  0x78  0xf5  0xff
		0xa0  0x97  0xbd  0xeb
		0xc7  0x34  0xad  0xa8
		0x8c  0x31  0x62  0x8f
	*/
	SM3_256(str, strlen((char*)str), H0);
	cout << "\"";
	for (int i = 0; str[i] != '\0'; i++) {
		cout << str[i];
	}
	cout << "\"的哈希值：" << endl;
	for (int i = 0; i < 32; i++) {
		printf("0x%x  ", H0[i]);
		if ((i + 1) % 4 == 0) {
			printf("\n");
		}
	}
	//预计算
	unsigned char str2[100] = { 0 };
	str2[0] = 'S';
	str2[1] = 'D';
	str2[2] = 'U';
	str2[3] = 0x80;
	str2[63] = 0x18;
	str2[64] = 'S';
	str2[65] = 'D';
	str2[66] = 'U';
	SM3_256(str2, 67, H1);
	cout << "Hash2：" << endl;
	for (int i = 0; i < 32; i++) {
		printf("0x%x  ", H1[i]);
		if ((i + 1) % 4 == 0) {
			printf("\n");
		}
	}

	unsigned int tmpHash[8] = { 0 };     
	unsigned int tmp = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 4 * i + 3; j >= 4 * i; j--) {
			tmp = H0[j];
			tmp = tmp * pow(256, (4 * i + 3 - j));
			tmpHash[i] += tmp;
		}
	}
	//填充m2
	int nSrcLen = strlen((char*)str);
	int NSrcLen = strlen((char*)str) + 64;
	unsigned int nGroupNum = (nSrcLen + 1 + 8 + 64) / 64;
	unsigned char* ucpMsgBuf = (unsigned char*)malloc(nGroupNum * 64);
	memset(ucpMsgBuf, 0, nGroupNum * 64);
	memcpy(ucpMsgBuf, str, nSrcLen);
	ucpMsgBuf[nSrcLen] = 0x80;
	int i = 0;
	for (i = 0; i < 8; i++) {
		ucpMsgBuf[nGroupNum * 64 - i - 1] = ((unsigned long long)(NSrcLen * 8) >> (i * 8)) & 0xFF;
	}
	_CF(ucpMsgBuf, tmpHash);
	free(ucpMsgBuf);

	for (i = 0; i < 8; i++) {
		H2[i * 4 + 0] = (unsigned char)((tmpHash[i] >> 24) & 0xFF);
		H2[i * 4 + 1] = (unsigned char)((tmpHash[i] >> 16) & 0xFF);
		H2[i * 4 + 2] = (unsigned char)((tmpHash[i] >> 8) & 0xFF);
		H2[i * 4 + 3] = (unsigned char)((tmpHash[i] >> 0) & 0xFF);
	}
	cout << "Hash3：" << endl;
	for (int i = 0; i < 32; i++) {
		printf("0x%x  ", H2[i]);
		if ((i + 1) % 4 == 0) {
			printf("\n");
		}
	}
	printf("\n");
	for (int i = 0; i < 32; i++) {
		if (H1[i] == H2[i]) {
			if (i != 31)
				continue;
			else
				printf("success！\n");
		} else {
			printf("fail！\n");
			break;
		}
	}
}
int main() {
	cout << "以下是函数自检" << endl;
	if (SM3_SelfTest() == 0) {
		cout << "通过自检！" << endl;
	}
	extension_attack();
	rho_collision();
	birthday_attack();
	
}
void BiToW(unsigned int Bi[], unsigned int W[])
{
	int i;
	unsigned int tmp;
	for (i = 0; i < 16; i += 4) {
		W[i] = Bi[i];
		W[i + 1] = Bi[i + 1];
		W[i + 2] = Bi[i + 2];
		W[i + 3] = Bi[i + 3];
	}
	for (i = 16; i <= 67; i++) {
		tmp = W[i - 16] ^ W[i - 9] ^ SM3_rotl32(W[i - 3], 15);
		W[i] = SM3_p1(tmp) ^ (SM3_rotl32(W[i - 13], 7)) ^ W[i - 6];
	}
}
/*****************************************************************
 Function: WToW1
 Description: calculate W1 from W
 Calls:
 Called By: SM3_compress
 Input: W[64]
 Output: W1[64]
 Return: null
 Others:
*****************************************************************/
void WToW1(unsigned int W[], unsigned int W1[])
{
	int i;
	//for (i = 0; i <= 63; i++) {
	//	W1[i] = W[i] ^ W[i + 4];
	//}
	for (i = 0; i < 64; i += 4) {
		W1[i] = W[i] ^ W[i + 4];
		W1[i + 1] = W[i + 1] ^ W[i + 5];
		W1[i + 2] = W[i + 2] ^ W[i + 6];
		W1[i + 3] = W[i + 3] ^ W[i + 7];
	}
}
/******************************************************************
 Function: CF
 Description: calculate the CF compress function and update V
 Calls:
 Called By: SM3_compress
 Input: W[64]
 W1[64]
 V[8]
 Output: V[8]
 Return: null
 Others:
********************************************************************/
void CF(unsigned int W[], unsigned int W1[], unsigned int V[]) {
	unsigned int SS1;
	unsigned int SS2;
	unsigned int TT1;
	unsigned int TT2;
	unsigned int A, B, C, D, E, F, G, H;
	unsigned int T = SM3_T1;
	unsigned int FF;
	unsigned int GG;
	int j;
	//reg init,set ABCDEFGH=V0
	A = V[0];
	B = V[1];
	C = V[2];
	D = V[3];
	E = V[4];
	F = V[5];
	G = V[6];
	H = V[7];
	for (j = 0; j <= 63; j++) {
		//SS1
		T = j == 0 ? SM3_T1 : (j == 16 ? SM3_rotl32(SM3_T2, 16) : SM3_rotl32(T, 1));
		SS1 = SM3_rotl32((SM3_rotl32(A, 12) + E + T), 7);
		//SS2
		SS2 = SS1 ^ SM3_rotl32(A, 12);
		//TT1
		FF = j <= 15 ? SM3_ff0(A, B, C) : SM3_ff1(A, B, C);
		TT1 = FF + D + SS2 + W1[j];
		//TT2
		GG = j <= 15 ? SM3_gg0(E, F, G) : SM3_gg1(E, F, G);
		TT2 = GG + H + SS1 + W[j];
		//D
		D = C;
		//C
		C = SM3_rotl32(B, 9);
		//B
		B = A;
		//A
		A = TT1;
		//H
		H = G;
		//G
		G = SM3_rotl32(F, 19);
		//F
		F = E;
		//E
		E = SM3_p0(TT2);
	}
	//update V
	V[0] = A ^ V[0];
	V[1] = B ^ V[1];
	V[2] = C ^ V[2];
	V[3] = D ^ V[3];
	V[4] = E ^ V[4];
	V[5] = F ^ V[5];
	V[6] = G ^ V[6];
	V[7] = H ^ V[7];
}
/******************************************************************************
 Function: BigEndian
 Description: U32 endian converse.GM/T 0004-2012 requires to use big-endian.
 if CPU uses little-endian, BigEndian function is a necessary
 call to change the little-endian format into big-endian format.
 Calls:
 Called By: SM3_compress, SM3_done
 Input: src[bytelen]
 bytelen
 Output: des[bytelen]
 Return: null
 Others: src and des could implies the same address
*******************************************************************************/
void BigEndian(unsigned char src[], unsigned int bytelen, unsigned char des[]) {
	unsigned char tmp = 0;
	unsigned int i = 0, loop_time = bytelen / 4;
	for (i = 0; i < loop_time; i++) {
		tmp = des[4 * i];
		des[4 * i] = src[4 * i + 3];
		src[4 * i + 3] = tmp;
		tmp = des[4 * i + 1];
		des[4 * i + 1] = src[4 * i + 2];
		des[4 * i + 2] = tmp;
	}
}
/******************************************************************************
 Function: SM3_init
 Description: initiate SM3 state
 Calls:
 Called By: SM3_256
 Input: SM3_STATE *md
 Output: SM3_STATE *md
 Return: null
 Others:
*******************************************************************************/
void SM3_init(SM3_STATE* md) {
	md->curlen = md->length = 0;
	md->state[0] = SM3_IVA;
	md->state[1] = SM3_IVB;
	md->state[2] = SM3_IVC;
	md->state[3] = SM3_IVD;
	md->state[4] = SM3_IVE;
	md->state[5] = SM3_IVF;
	md->state[6] = SM3_IVG;
	md->state[7] = SM3_IVH;
}
/******************************************************************************
 Function: SM3_compress
 Description: compress a single block of message
 Calls: BigEndian
 BiToW
 WToW1
 CF
 Called By: SM3_256
 Input: SM3_STATE *md
 Output: SM3_STATE *md
 Return: null
 Others:
*******************************************************************************/
void SM3_compress(SM3_STATE* md) {
	unsigned int W[68];
	unsigned int W1[64];
	//if CPU uses little-endian, BigEndian function is a necessary call
	BigEndian(md->buf, 64, md->buf);
	BiToW((unsigned int*)md->buf, W);
	WToW1(W, W1);
	CF(W, W1, md->state);
}
/******************************************************************************
 Function: SM3_process
 Description: compress the first (len/64) blocks of message
 Calls: SM3_compress
 Called By: SM3_256
 Input: SM3_STATE *md
 unsigned char buf[len] //the input message
 int len //bytelen of message
 Output: SM3_STATE *md
 Return: null
 Others:
*******************************************************************************/
void SM3_process(SM3_STATE* md, unsigned char* buf, int len) {
	while (len--) {
		/* copy byte */
		md->buf[md->curlen] = *buf++;
		md->curlen++;
		/* is 64 bytes full? */
		if (md->curlen == 64) {
			SM3_compress(md);
			md->length += 512;
			md->curlen = 0;
		}
	}
}
/******************************************************************************
 Function: SM3_done
 Description: compress the rest message that the SM3_process has left behind
 Calls: SM3_compress
 Called By: SM3_256
 Input: SM3_STATE *md
 Output: unsigned char *hash
 Return: null
 Others:
*******************************************************************************/
void SM3_done(SM3_STATE* md, unsigned char hash[])
{
	int i;
	unsigned char tmp = 0;
	/* increase the bit length of the message */
	md->length += md->curlen << 3;
	/* append the '1' bit */
	md->buf[md->curlen] = 0x80;
	md->curlen++;
	/* if the length is currently above 56 bytes, appends zeros till
	it reaches 64 bytes, compress the current block, creat a new
	block by appending zeros and length,and then compress it
	*/
	if (md->curlen > 56) {
		for (; md->curlen < 64;) {
			md->buf[md->curlen] = 0;
			md->curlen++;
		}
		SM3_compress(md);
		md->curlen = 0;
	}
	/* if the length is less than 56 bytes, pad upto 56 bytes of zeroes */
	for (; md->curlen < 56;) {
		md->buf[md->curlen] = 0;
		md->curlen++;
	}
	/* since all messages are under 2^32 bits we mark the top bits zero */
	for (i = 56; i < 60; i++) {
		md->buf[i] = 0;
	}
	/* append length */
	md->buf[63] = md->length & 0xff;
	md->buf[62] = (md->length >> 8) & 0xff;
	md->buf[61] = (md->length >> 16) & 0xff;
	md->buf[60] = (md->length >> 24) & 0xff;
	SM3_compress(md);
	/* copy output */
	memcpy(hash, md->state, SM3_len / 8);
	BigEndian(hash, SM3_len / 8, hash);//if CPU uses little-endian, BigEndian function is a necessary call
}
/******************************************************************************
 Function: SM3_256
 Description: calculate a hash value from a given message
 Calls: SM3_init
 SM3_process
 SM3_done
 Called By:
 Input: unsigned char buf[len] //the input message
 int len //bytelen of the message
 Output: unsigned char hash[32]
 Return: null
 Others:
*******************************************************************************/
void SM3_256(unsigned char buf[], int len, unsigned char hash[])
{
	SM3_STATE md;
	SM3_init(&md);
	SM3_process(&md, buf, len);
	SM3_done(&md, hash);
}
/******************************************************************************
 Function: SM3_SelfTest
 Description: test whether the SM3 calculation is correct by comparing
 the hash result with the standard result
 Calls: SM3_256
 Called By:
 Input: null
 Output: null
 Return: 0 //the SM3 operation is correct
 1 //the sm3 operation is wrong
 Others:
*******************************************************************************/
int SM3_SelfTest() {
	unsigned int i = 0, a = 1, b = 1;
	unsigned char Msg1[3] = { 0x61,0x62,0x63 };
	int MsgLen1 = 3;
	unsigned char MsgHash1[32] = { 0 };
	unsigned char StdHash1[32] =
	{ 0x66,0xC7,0xF0,0xF4,0x62,0xEE,0xED,0xD9,0xD1,0xF2,0xD4,0x6B,0xDC,0x10,0xE4,0xE2,

	  0x41,0x67,0xC4,0x87,0x5C,0xF2,0xF7,0xA2,0x29,0x7D,0xA0,0x2B,0x8F,0x4B,0xA8,0xE0
	};
	unsigned char Msg2[64] =
	{ 0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,

	  0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,

	  0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,

	  0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64
	};
	int MsgLen2 = 64;
	unsigned char MsgHash2[32] = { 0 };
	unsigned char StdHash2[32] =
	{ 0xde,0xbe,0x9f,0xf9,0x22,0x75,0xb8,0xa1,0x38,0x60,0x48,0x89,0xc1,0x8e,0x5a,0x4d,

	  0x6f,0xdb,0x70,0xe5,0x38,0x7e,0x57,0x65,0x29,0x3d,0xcb,0xa3,0x9c,0x0c,0x57,0x32
	};
	SM3_256(Msg1, MsgLen1, MsgHash1);
	SM3_256(Msg2, MsgLen2, MsgHash2);
	a = memcmp(MsgHash1, StdHash1, SM3_len / 8);
	b = memcmp(MsgHash2, StdHash2, SM3_len / 8);
	return (a == 0) && (b == 0) ? 0 : 1;
}
static unsigned int T[64] = { 0 };
static void _init_T() {
	int i = 0;
	for (; i < 16; i++)
		T[i] = 0x79CC4519;
	for (; i < 64; i++)
		T[i] = 0x7A879D8A;
}
void _CF(unsigned char* ucpSrcMsg, unsigned int tmpHash[8]) {
	unsigned int W[68] = { 0 };
	unsigned int W1[64] = { 0 };
	_init_T();
	//message extension
	int j = 0;
	for (j = 0; j < 16; j++)
	{
		W[j] = ((unsigned int)ucpSrcMsg[j * 4 + 0] << 24) & 0xFF000000
			| ((unsigned int)ucpSrcMsg[j * 4 + 1] << 16) & 0x00FF0000
			| ((unsigned int)ucpSrcMsg[j * 4 + 2] << 8) & 0x0000FF00
			| ((unsigned int)ucpSrcMsg[j * 4 + 3] << 0) & 0x000000FF;
	}

	for (j = 16; j < 68; j++) {
		W[j] = SM3_p1(W[j - 16] ^ W[j - 9] ^ (SM3_rotl32(W[j - 3], 15))) ^ (SM3_rotl32(W[j - 13], 7)) ^ W[j - 6];
	}

	for (j = 0; j < 64; j++) {
		W1[j] = W[j] ^ W[j + 4];
	}

	unsigned int A_G[8] = { 0 };
	for (j = 0; j < 8; j++) {
		A_G[j] = tmpHash[j];
	}

	unsigned int SS1 = 0, SS2 = 0, TT1 = 0, TT2 = 0, ff = 0, gg = 0;

	for (j = 0; j < 64; j++) {
		if (j >= 0 && j < 16) {
			ff = SM3_ff0(A_G[0], A_G[1], A_G[2]);
			gg = SM3_gg0(A_G[4], A_G[5], A_G[6]);
		} else {
			ff = SM3_ff1(A_G[0], A_G[1], A_G[2]);
			gg = SM3_gg1(A_G[4], A_G[5], A_G[6]);
		}
		SS1 = SM3_rotl32((SM3_rotl32(A_G[0], 12) + A_G[4] + SM3_rotl32(T[j], j % 32)), 7);
		SS2 = SS1 ^ (SM3_rotl32(A_G[0], 12));
		TT1 = ff + A_G[3] + SS2 + W1[j];
		TT2 = gg + A_G[7] + SS1 + W[j];
		A_G[3] = A_G[2];
		A_G[2] = SM3_rotl32(A_G[1], 9);
		A_G[1] = A_G[0];
		A_G[0] = TT1;
		A_G[7] = A_G[6];
		A_G[6] = SM3_rotl32(A_G[5], 19);
		A_G[5] = A_G[4];
		A_G[4] = SM3_p0(TT2);
	}

	for (j = 0; j < 8; j++) {
		tmpHash[j] = A_G[j] ^ tmpHash[j];
	}
}