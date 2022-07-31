import random
import socket
from SM3 import *
from func_lib import *

#椭圆曲线参数
P  = 115792089237316195423570985008687907853269984665640564039457584007908834671663    
A  = 0
B  = 7
N  = 115792089237316195423570985008687907852837564279074904382605163141518161494337


#端口设置
host, port = '', 1000
Party2     = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
Party2.bind((host, port))

print("P2 has connected!")
#generate sub private key d2
d_2 = random.randint(1, N - 1)

#receive T1 from P1
T1x, Address = Party2.recvfrom(1024)
T1x          = int(T1x.decode(), 16)
T1y, Address = Party2.recvfrom(1024)
T1y          = int(T1y.decode(), 16)
T1           = (T1x, T1y)

#compute T2 = d2^(-1) * T1
T2           = elliptic_multiply(mod_inverse(d_2, P), T1)
T2x, T2y     = hex(T2[0]), hex(T2[1])

Party2.sendto(T2x.encode('utf-8'), Address)
Party2.sendto(T2y.encode('utf-8'), Address)

print("Closed!")