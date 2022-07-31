import socket
import json
from os.path import commonprefix
from SM3 import *

#公私钥对
sk_server, pk = 2200, 1376666

#端口设置
host, port = '', 1000

#绑定端口
Client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
Client.bind((host, port))
print("The Server connects successfully!")

#Process data info
k, Address = Client.recvfrom(1024)
k          = int(k.decode(),16)
v, Address = Client.recvfrom(1024)
v          = int(v.decode(),16)

# create key-value table
KV_table = []

#divide the table into 2^16 sets according to the key k_i
for i in range(pow(2, 16)):
    h = SM3(str(i) + str(i)).hash()
    KV_table.append(h)   
S = []
for item in KV_table:
    if item[:2] == k:
        S.append((pow(int(item, 16),sk_server))%pk)

print("data set S = ",S)

#compute h^(ab)
h__ab = (pow(v, sk_server)) % pk

#send h^(ab) and data set S to the client
#将列表转换成json结构
json_S = json.dumps(S)
Client.sendto(hex(h__ab).encode(), Address)
Client.sendto(json_S.encode('utf-8'), Address)
Client.close()

print("The Server has closed!")