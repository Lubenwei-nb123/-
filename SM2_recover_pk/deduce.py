#coding:utf-8
import secrets
from hashlib import sha256
import func_lib


#参数选取
Gx = 55066263022277343669578718895168534326250603453777594175500187360389116729240
Gy = 32670510020758816978083085130507043184471273380659243275938904335757337482424
G = (Gx, Gy)
P = 115792089237316195423570985008687907853269984665640564039457584007908834671663
N = 115792089237316195423570985008687907852837564279074904382605163141518161494337

#生成公私钥对
def generate_key():
    sk = int(secrets.token_hex(32), 16)
    #这里使用secrets模块中的加密安全随机数生成器，提高安全性.
    pk = func_lib.elliptic_multiply(sk, G)
    return sk, pk

#sha256哈希函数
def hash(message):
    return int(sha256(message.encode('utf-8')).hexdigest(), 16)

#签名
def sign(private_key, message):
    e = hash(message)
    k = secrets.randbelow(P)
    random_point = func_lib.elliptic_multiply(k, G)
    r = random_point[0] % P
    s = func_lib.mod_inverse(k, N) * (e + r*private_key) % N
    return (r, s)

#根据签名恢复公钥
def recover_pk(sig, msg):
    r  = sig[0]
    s  = sig[1]
    x  = r % P
    y2 = ((x**3)+7)
    y  = func_lib.Tonelli_Shanks(y2, P)

    e  = hash(msg)
    POINT_1 = (x, y)
    POINT_2 = (x, P-y)

    #y1:
    skG   = func_lib.elliptic_multiply(s%N,POINT_1)
    eG    = func_lib.elliptic_multiply(e%N,G)
    negeG = (eG[0],P-eG[1])
    skGeG = func_lib.elliptic_add(skG,negeG)
    pk1   = func_lib.elliptic_multiply(func_lib.mod_inverse(r,N),skGeG)

    # y2:
    skG   = func_lib.elliptic_multiply(s%N,POINT_2)
    skGeG = func_lib.elliptic_add(skG,negeG)
    pk2   = func_lib.elliptic_multiply(func_lib.mod_inverse(r,N),skGeG)
    return pk1,pk2

sk, pk = generate_key()

print('real pk：',pk)

msg = "SDU"
sig = sign(sk, msg)

print("sig(sk, msg): ", sig)
pk1, pk2 = recover_pk(sig, msg)
print('deduced pk：')
print(pk1)
print(pk2)

