import random
from SM3 import *
from func_lib import *
#参数设置
P = 115792089237316195423570985008687907853269984665640564039457584007908834671663
A = 0
B = 7
N = 115792089237316195423570985008687907852837564279074904382605163141518161494337
Gx = 55066263022277343669578718895168534326250603453777594175500187360389116729240
Gy = 32670510020758816978083085130507043184471273380659243275938904335757337482424
G = (Gx, Gy)

#公私钥对生成
d = random.randint(1, N - 2)
pk = elliptic_multi(d, G, A, P)

print("d:\n",d)
print("P:\n",pk)

#哈希函数
def elliptic_Hash(input):
    r = [float("inf"), float("inf")]
    for s in input:
        x = int(SM3(str(s, 'UTF-8')).hash(), 16)
        y = elliptic_mod(x ** 3 + A * x + B, P)
        r = elliptic_addmod(r, [x, y], A, P)
    return r

set = (b'4396', b'1376666', b'15')
r = elliptic_Hash(set)

print("str set:\n",set)
print("elliptic Hash of set:\n",r)