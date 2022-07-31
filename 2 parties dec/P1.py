import random
import sys
import math
import socket
from SM3 import *
from func_lib import *

#椭圆曲线参数
P  = 115792089237316195423570985008687907853269984665640564039457584007908834671663    
A  = 0
B  = 7
N  = 115792089237316195423570985008687907852837564279074904382605163141518161494337

#端口设置
host, port = '127.0.0.1', 1000
Address = (host, port)
Party1 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    Party1.connect((host, port))
    print("P1 has connected!")
except Exception:
    print('P1 failed to connect!')
    sys.exit()
else:
    #generate sub private key d_1
    d_1 = random.randint(1, N - 1)
    
    #get ciphertext C = C_1 || C_2 || C_3
    C_1  = (0x123456789, 0x233333333)
    _C_1 = (C_1[0], - C_1[1])
    C_2  = 0x131415926
    C_3  = 0x777777777
    
    #compute T_1 = d_1^(-1) * C_1
    T_1      = elliptic_multiply(mod_inverse(d_1, P), C_1)
    T1x, T1y = hex(T_1[0]), hex(T_1[1])
    k_len    = len(hex(C_2)[2:])*4
    
    #send T1 to 2
    Party1.sendto(T1x.encode('utf-8'), Address)
    Party1.sendto(T1y.encode('utf-8'), Address)

    #receive T2 from P2
    T2x, Address = Party1.recvfrom(1024)
    T2x          = int(T2x.decode(), 16)
    T2y, Address = Party1.recvfrom(1024)
    T2y          = int(T2y.decode(), 16)
    T_2          = (T2x, T2y)
    
    #compute T2 - C1
    x_2, y_2 = elliptic_add(T_2, _C_1)
    x_2, y_2 = '{:0256b}'.format(x_2), '{:0256b}'.format(y_2)
    t        = KDF(x_2 + y_2, k_len)
    __M      = C_2 ^ int(t, 2)
    u        = SM3(hex(int(x_2,2))[2:] + hex(__M)[2:] + hex(int(y_2,2))[2:]).hash()
    print(hex(__M)[2:])
    Party1.close()