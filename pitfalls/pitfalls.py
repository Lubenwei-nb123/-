from sqlite3 import register_converter
from SM3 import *
import random
from func_lib import *

#y^2=x^3+7
A = 0
B = 7
# 椭圆曲线的阶
N = 115792089237316195423570985008687907852837564279074904382605163141518161494337
#椭圆曲线参数
Gx = 55066263022277343669578718895168534326250603453777594175500187360389116729240
Gy = 32670510020758816978083085130507043184471273380659243275938904335757337482424
G = (Gx, Gy)

k = random.randint(1, N - 1)
# SM2 signature
def SM2_sign(msg):
    global N, G, k, d
    P_A = elliptic_multiply(d,G)
    ID_A = '123456789'
    Z_A = SM3(str(len(ID_A)) + ID_A + str(A) + str(B) + str(Gx) + str(Gy) + str(P_A[0]) + str(P_A[1])).hash()
    kG = elliptic_multiply(k, G)

    _M = Z_A + msg #_M = Z_A||M 
    e = SM3(_M).hash()
    r = (int(e, 16) + kG[0]) % N
    s = (mod_inverse(1 + d, N) * (k - r * d)) % N
    while r == 0 or r + k == N or s == 0:
        k = random.randint(1, N - 1)
        kG = elliptic_multiply(k, G)
        r = (int(e, 16) + kG[0]) % N
        s = (mod_inverse(1 + d, N) * (k - r * d)) % N
    return r,s

# ECDSA signature
def ECDSA_sign(msg):
    global N, G, k, d
    P = elliptic_multiply(k,G)
    r = P[0] % N
    if r == 0:
        return -1
    e = hash(msg)
    s = (mod_inverse(k, N) * (e + d * r)) % N
    return r,s


m = '202000460122'
e = hash(m)
d = 4396

#leaking k
sig = SM2_sign(m)
corrupted_d_A = (mod_inverse(sig[0] + sig[1], N) * (k - sig[1]))%N
print('leaking k:')
print("recover d:", corrupted_d_A)

#reusing k
m1 = '202000460122'
m2 = '202000460123'
sig1 = SM2_sign(m1)
sig2 = SM2_sign(m2)
r1, s1, r2, s2 = sig1[0], sig1[1], sig2[0], sig2[1]
recover_d = ((s2 - s1) * mod_inverse((s1 - s2 + r1 - r2), N)) % N
print('\nreusing k:')
print("recover d:", d)

#reusing k by two users
r1,s1 = SM2_sign(m1)
r2,s2 = SM2_sign(m2)
sk1 = ((k - s1) * mod_inverse(s1 + r1,N))%N
sk2 = ((k - s2) * mod_inverse(s2 + r2,N))%N
print('\nreusing k by two users:')
print("recover d1:",sk1)
print("recover d2:",sk2)

#same d and k with ECDSA
e1 = hash(m)
r1,s1 = SM2_sign(m1)
r2,s2 = ECDSA_sign(m2)
sk = ((s1 * s2 - e1) * mod_inverse((r1 - s1 * s2 - s1 * r2), N)) % N
print('\nsame d and k with ECDSA:')
print("recover d:",d)