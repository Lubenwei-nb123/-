#include "miracl.h"
#include "mirdef.h"
#include "SM2.h"
#include "kdf.h"

int testPoint(epoint* point) {
	big x, y, x_3, tmp;
	x = mirvar(0);
	y = mirvar(0);
	x_3 = mirvar(0);
	tmp = mirvar(0);
	//test if y^2=x^3+ax+b
	epoint_get(point, x, y);
	powmod(x, 3, para_p, x_3); //x_3=x^3 mod p
	multiply(x, para_a, x); //x=a*x
	divide(x, para_p, tmp); //x=a*x mod p , tmp=a*x/p
	add(x_3, x, x); //x=x^3+ax
	add(x, para_b, x); //x=x^3+ax+b
	divide(x, para_p, tmp); //x=x^3+ax+b mod p
	powmod(y, 2, para_p, y); //y=y^2 mod p
	if (x -> w != y -> w)
		return ERR_NOT_VALID_POINT;
	else
		return 0;
}

int testPublicKey(epoint* pubKey) {
	big x, y, x_3, tmp;
	epoint* nP;
	x = mirvar(0);
	y = mirvar(0);
	x_3 = mirvar(0);
	tmp = mirvar(0);
	nP = epoint_init();
	//test if the pubKey is the point at infinity
	if (point_at_infinity(pubKey))// if pubKey is point at infinity, return error;
		return ERR_INFINITY_POINT;
	//test if x<p and y<p both hold
	epoint_get(pubKey, x, y);
	if ((x->w < para_p->w)|| (y->w < para_p->w))
		return ERR_NOT_VALID_ELEMENT;
	if (testPoint(pubKey) != 0)
		return ERR_NOT_VALID_POINT;
	//test if the order of pubKey is equal to n
	ecurve_mult(para_n, pubKey, nP); // nP=[n]P
	if (!point_at_infinity(nP)) // if np is point NOT at infinity, return error;
		return ERR_ORDER;
	return 0;
}

int testNull(unsigned char array[], int len) {
	int i = 0;
	for (i = 0; i < len; i++) {
		if (array[i] != 0x00)
			return 0;
	}
	return 1;
}

int SM2_init() {
	epoint* nG;
	para_p = mirvar(0);
	para_a = mirvar(0);
	para_b = mirvar(0);
	para_n = mirvar(0);
	para_Gx = mirvar(0);
	para_Gy = mirvar(0);
	para_h = mirvar(0);
	G = epoint_init();
	nG = epoint_init();
	bytes_to_big(SM2_NUMWORD, SM2_p, para_p);
	bytes_to_big(SM2_NUMWORD, SM2_a, para_a);
	bytes_to_big(SM2_NUMWORD, SM2_b, para_b);
	bytes_to_big(SM2_NUMWORD, SM2_n, para_n);
	bytes_to_big(SM2_NUMWORD, SM2_Gx, para_Gx);
	bytes_to_big(SM2_NUMWORD, SM2_Gy, para_Gy);
	bytes_to_big(SM2_NUMWORD, SM2_h, para_h);
	ecurve_init(para_a, para_b, para_p, MR_PROJECTIVE);
	if (!epoint_set(para_Gx, para_Gy, 0, G))//initialise point G
	{
		return ERR_ECURVE_INIT;
	}
	ecurve_mult(para_n, G, nG);
	if (!point_at_infinity(nG)) //test if the order of the point is n
	{
		return ERR_ORDER;
	}
	return 0;
}

int SM2KeyGeneration(big priKey, epoint* pubKey) {
	int i = 0;
	big x, y;
	x = mirvar(0);
	y = mirvar(0);
	ecurve_mult(priKey, G, pubKey);
	epoint_get(pubKey, x, y);
	if (testPublicKey(pubKey) != 0)
		return 1;
	else
		return 0;
}

int SM2_encrypt(unsigned char* randK, epoint* pubKey, unsigned char M[], int klen, unsigned char C[]) {
	big C1x, C1y, x2, y2, rand;
	epoint* C1, * kP, * S;
	int i = 0;
	unsigned char x2y2[SM2_NUMWORD * 2] = { 0 };
	SM3_STATE md;
	C1x = mirvar(0);
	C1y = mirvar(0);
	x2 = mirvar(0);
	y2 = mirvar(0);
	rand = mirvar(0);
	C1 = epoint_init();
	kP = epoint_init();
	S = epoint_init();
	//Step2. calculate C1=[k]G=(rGx,rGy)
	bytes_to_big(SM2_NUMWORD, randK, rand);
	ecurve_mult(rand, G, C1); //C1=[k]G
	epoint_get(C1, C1x, C1y);
	big_to_bytes(SM2_NUMWORD, C1x, C, 1);
	big_to_bytes(SM2_NUMWORD, C1y, C + SM2_NUMWORD, 1);
	//Step3. test if S=[h]pubKey if the point at infinity
	ecurve_mult(para_h, pubKey, S);
	if (point_at_infinity(S))// if S is point at infinity, return error;
		return ERR_INFINITY_POINT;
	//Step4. calculate [k]PB=(x2,y2)
	ecurve_mult(rand, pubKey, kP); //kP=[k]P
	epoint_get(kP, x2, y2);
	//Step5. KDF(x2||y2,klen)
	big_to_bytes(SM2_NUMWORD, x2, x2y2, 1);
	big_to_bytes(SM2_NUMWORD, y2, x2y2 + SM2_NUMWORD, 1);
	SM3_KDF(x2y2, SM2_NUMWORD * 2, klen, C + SM2_NUMWORD * 3);
	if (testNull(C + SM2_NUMWORD * 3, klen) != 0)
		return ERR_ARRAY_NULL;
	//Step6. C2=M^t
	for (i = 0; i < klen; i++) {
		C[SM2_NUMWORD * 3 + i] = M[i] ^ C[SM2_NUMWORD * 3 + i];
	}
	//Step7. C3=hash(x2,M,y2)
	SM3_init(&md);
	SM3_process(&md, x2y2, SM2_NUMWORD);
	SM3_process(&md, M, klen);
	SM3_process(&md, x2y2 + SM2_NUMWORD, SM2_NUMWORD);
	SM3_done(&md, C + SM2_NUMWORD * 2);
	return 0;
}