import socket
from random import randint
from func_lib import *


#椭圆曲线参数
P  = 115792089237316195423570985008687907853269984665640564039457584007908834671663    
A  = 0
B  = 7
N  = 115792089237316195423570985008687907852837564279074904382605163141518161494337
Gx = 55066263022277343669578718895168534326250603453777594175500187360389116729240
Gy = 32670510020758816978083085130507043184471273380659243275938904335757337482424
G  = (Gx, Gy)
_G = (Gx, -Gy) 

#端口设置
host, port = '', 1000
Party2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
Party2.bind((host, port))

print("P2 has connected!")

#generate sub private key d2
d_2 = randint(1, N - 1)

#receive P1=(x,y) from P1
P1x, Address = Party2.recvfrom(1024)
P1x          = int(P1x.decode(),16)
P1y, Address = Party2.recvfrom(1024)
P1y          = int(P1y.decode(),16)

#generate shared public key P
P1 = (P1x,P1y)
P  = elliptic_multiply(mod_inverse(d_2,P), P1)
P  = elliptic_add(P, _G)

#receive Q1 and e from P1
Q1x, Address = Party2.recvfrom(1024)
Q1x          = int(Q1x.decode(),16)
Q1y,Address  = Party2.recvfrom(1024)
Q1y          = int(Q1y.decode(),16)
Q1           = (Q1x, Q1y)
e, Address   = Party2.recvfrom(1024)
e            = int(e.decode(),16)

#randomly generate k2
k_2 = randint(1, N - 1)

#randomly generate k3
k_3 = randint(1, N - 1)

#compute Q2 = k2 * G
Q2 = elliptic_multiply(k_2, G)

#compute (x1,y1) = k3 * Q1 + Q2
x1,y1 = elliptic_multiply(k_3, Q1)
tmp   = (x1, y1)
x1,y1 = elliptic_add(tmp, Q2)
r     = (x1 + e) % N
s2    = (d_2 * k_3)%N
s3    = (d_2 * (r + k_2))%N

#send r, s2 and s3 to P1
Party2.sendto(hex(r).encode(),Address)
Party2.sendto(hex(s2).encode(),Address)
Party2.sendto(hex(s3).encode(),Address)

print("Closed!")