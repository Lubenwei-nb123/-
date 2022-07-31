from SM3 import *
import json
import socket
from os.path import commonprefix

sk_server = 7
n = 23
host = ''
port = 1234
Client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
Client.bind((host, port))

print("Server connected!")

# 从客户端接收1024B的数据
k, Address = Client.recvfrom(1024)
k = int(k.decode(),16)
v, Address = Client.recvfrom(1024)
v = int(v.decode(),16)

# create key-value table
KV_table = []

#divide the table into 2^16 sets according to the key k_i
for i in range(20000 ,20000 + pow(2,16)):
    h = SM3(str(i) + str(i)).hash()
    KV_table.append(h)   
S = []
for m in KV_table:
    if m[:2] == k:
        S.append((pow(int(m,16),sk_server))%n)

print("S:",S)

#compute h^(ab)
h__ab=(pow(v, sk_server))%n

#send h^(ab) and data set S to the client
Client.sendto(hex(h__ab).encode(), Address)
#将列表转换成json结构
json_v = json.dumps(S)
Client.sendto(json_v.encode('utf-8'), Address)
Client.close()

print("Server closed!")