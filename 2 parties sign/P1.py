import sys
import socket
import binascii
import random
from func_lib import *
from SM3 import *

#椭圆曲线参数
P  = 115792089237316195423570985008687907853269984665640564039457584007908834671663    
A  = 0
B  = 7
N  = 115792089237316195423570985008687907852837564279074904382605163141518161494337

Gx = 55066263022277343669578718895168534326250603453777594175500187360389116729240
Gy = 32670510020758816978083085130507043184471273380659243275938904335757337482424
G  = (Gx, Gy)


#端口设置
host, port = '127.0.0.1', 1000
Party1     = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    Party1.connect((host, port))
    print("P1 has connected!")
except Exception:
    print('P1 failed to connect!')
    sys.exit()
else:
    #generate sub private key d1
    d_1 = random.randint(1, N - 1)
    
    #compute P1 = d1^(-1) G
    P1       = elliptic_multiply(mod_inverse(d_1,P), G)
    P1x, P1y = hex(P1[0]), hex(P1[1])
    
    #send P1 to P2
    Address = (host, port)
    Party1.sendto(P1x.encode('utf-8'), Address)
    Party1.sendto(P1y.encode('utf-8'), Address)

    #compute Z
    M = "SDU is NO.1 university!"
    Z = hex(int(binascii.b2a_hex(M.encode()).decode(), 16)).upper()[2:]
    e = SM3(Z).hash()
    
    #randomly generate k1
    k_1 = random.randint(1, N - 1)

    #compute Q1 = k1 * G
    Q1       = elliptic_multiply(k_1, G)
    Q1x, Q1y = hex(Q1[0]), hex(Q1[1])

    #send Q1 and e to P2
    Party1.sendto(Q1x.encode('utf-8'), Address)
    Party1.sendto(Q1y.encode('utf-8'), Address)
    Party1.sendto(e.encode('utf-8'), Address)

    #receive r,s_2, s_3 from P2
    r, Address   = Party1.recvfrom(1024)
    r            = int(r.decode(),16)
    s_2, Address = Party1.recvfrom(1024)
    s_2          = int(s_2.decode(),16)
    s_3, Address = Party1.recvfrom(1024)
    s_3          = int(s_3.decode(),16)

    #compute s
    s = ((d_1 * k_1) * s_2 + d_1 * s_3 - r) % N
    if s!=0 or s!= N - r:
        print("2 parties signature:", (hex(r),hex(s)))
    Party1.close()