import random
from func_lib import *
#椭圆曲线参数
#y^2=x^3+7
A = 0
B = 7
Gx = 55066263022277343669578718895168534326250603453777594175500187360389116729240
Gy = 32670510020758816978083085130507043184471273380659243275938904335757337482424
G = (Gx, Gy)
N = 115792089237316195423570985008687907852837564279074904382605163141518161494337

#公私钥生成
d = 4396
P = elliptic_multiply(d, G)

#签名
def sign(m):
    global N, G, d, e
    k = random.randint(0,N-1) # k <- Zn*
    R = elliptic_multiply(k,G) #R = kG
    r = R[0] % N #r = x mod n
    s = (mod_inverse(k, N) * (e + d * r)) % N #s = k^-1(e+dr)modn
    return r,s

#验签
def verification(e, sig, P):
    global N,G,m
    w = mod_inverse(sig[1],N)
    r1, _ = elliptic_add(elliptic_multiply((e * w) % N,G),elliptic_multiply((sig[0] * w) % N,P))
    if(r1 % N == sig[0]):
        return True
    else:
        return False

def forge():
    global N, G, P
    u = random.randint(1,N-1)
    v = random.randint(1,N-1)
    R = elliptic_add(elliptic_multiply(u,G),elliptic_multiply(v,P))
    rf = R[0] % N # r' = x' (modn)
    e1 = (rf * u * mod_inverse(v, N)) % N
    sf = (rf * mod_inverse(v, N)) % N
    sig = (rf, sf)
    print('make up fake hash value of the message:',e1)
    print('signature forged:', sig)
    if(verification(e1, sig, P)):
        print('valid signature!')
    else:
        print('invalid signature!')

m = 'SDU is No.1 university!'
e = hash(m)
sig = sign(m)

print("Signature is:",sig)
if(verification(e, sig, P)):
    print('pass verification!')
else: print('fail to pass verification!')

forge()