#include "SM3_advanced.h"
#include<iostream>
using namespace std;
static void _CF(unsigned char* ucpSrcMsg, unsigned int nHash[8]);
int main() {
	//这里是攻击代码
	unsigned char Hash[32] = { 0 };
	unsigned char Hash2[32] = { 0 };
	unsigned char Hash3[32] = { 0 };
	unsigned int* hash_len = NULL;
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
	SM3_256(str, strlen((char*)str), Hash);
	cout << "\"";
	for (int i = 0; str[i] != '\0'; i++) {
		cout << str[i];
	}
	cout << "\"的哈希值：" << endl;
	for (int i = 0; i < 32; i++) {
		printf("0x%x  ", Hash[i]);
		if ((i + 1) % 4 == 0) {
			printf("\n");
		}
	}
	//预计算hash(m1||m2),其中str2=m1||m2
	unsigned char str2[100] = { 0 };
	str2[0] = 'S';
	str2[1] = 'D';
	str2[2] = 'U';
	str2[3] = 0x80;
	str2[63] = 0x18;
	str2[64] = 'S';
	str2[65] = 'D';
	str2[66] = 'U';
	SM3_256(str2, 67, Hash2);
	cout << "Hash2：" << endl;
	for (int i = 0; i < 32; i++) {
		printf("0x%x  ", Hash2[i]);
		if ((i + 1) % 4 == 0) {
			printf("\n");
		}
	}

	//长度扩展攻击，不知道原始数据m1，但知道后添加在尾部的数据m2及数据总长度NSrcLen
	unsigned int tempHash[8] = { 0 };     //经历完一轮压缩函数的哈希值
	unsigned int temp = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 4 * i + 3; j >= 4 * i; j--) {
			temp = Hash[j];
			temp = temp * pow(256, (4 * i + 3 - j));
			tempHash[i] += temp;
		}
	}
	//对m2进行消息填充
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
	//执行压缩函数，两个输入分别是上一轮压缩函数的哈希值和要添加到尾部的消息的填充值
	//for (int i = 0; i < 64; i++) {
	//	printf("%x", ucpMsgBuf[i]);
	//}
	_CF(ucpMsgBuf, tempHash);
	free(ucpMsgBuf);
	for (i = 0; i < 8; i++) {
		Hash3[i * 4 + 0] = (unsigned char)((tempHash[i] >> 24) & 0xFF);
		Hash3[i * 4 + 1] = (unsigned char)((tempHash[i] >> 16) & 0xFF);
		Hash3[i * 4 + 2] = (unsigned char)((tempHash[i] >> 8) & 0xFF);
		Hash3[i * 4 + 3] = (unsigned char)((tempHash[i] >> 0) & 0xFF);
	}
	cout << "Hash3：" << endl;
	for (int i = 0; i < 32; i++) {
		printf("0x%x  ", Hash3[i]);
		if ((i + 1) % 4 == 0) {
			printf("\n");
		}
	}
	printf("\n");
	for (int i = 0; i < 32; i++) {
		if (Hash2[i] == Hash3[i]) {
			if (i != 31)
				continue;
			else
				printf("伪造成功！");
		} else {
			printf("伪造失败！");
			break;
		}
	}
}
/****************************************************************
 Function: BiToW
 Description: calculate W from Bi
 Calls:
 Called By: SM3_compress
 Input: Bi[16] //a block of a message
Output: W[64]
 Return: null
 Others:
****************************************************************/
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
void _CF(unsigned char* ucpSrcMsg, unsigned int nHash[8]) {
	unsigned int W68[68] = { 0 };
	unsigned int W64[64] = { 0 };
	_init_T();
	//message extension
	int j = 0;
	for (j = 0; j < 16; j++)
	{
		W68[j] = ((unsigned int)ucpSrcMsg[j * 4 + 0] << 24) & 0xFF000000
			| ((unsigned int)ucpSrcMsg[j * 4 + 1] << 16) & 0x00FF0000
			| ((unsigned int)ucpSrcMsg[j * 4 + 2] << 8) & 0x0000FF00
			| ((unsigned int)ucpSrcMsg[j * 4 + 3] << 0) & 0x000000FF;
	}

	for (j = 16; j < 68; j++) {
		W68[j] = SM3_p1(W68[j - 16] ^ W68[j - 9] ^ (SM3_rotl32(W68[j - 3], 15))) ^ (SM3_rotl32(W68[j - 13], 7)) ^ W68[j - 6];
	}

	for (j = 0; j < 64; j++) {
		W64[j] = W68[j] ^ W68[j + 4];
	}

	//iterative process
	unsigned int A_G[8] = { 0 };
	for (j = 0; j < 8; j++) {
		A_G[j] = nHash[j];
	}

	//tempporary variable
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
		TT1 = ff + A_G[3] + SS2 + W64[j];
		TT2 = gg + A_G[7] + SS1 + W68[j];
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
		nHash[j] = A_G[j] ^ nHash[j];
	}
}