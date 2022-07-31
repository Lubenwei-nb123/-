from SM3 import *
from func_lib import *
import sys
import json
import socket
from os.path import commonprefix

sk_client = 5
n = 23
account = 7777
password = 4396

host = '127.0.0.1'
port = 1234
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    client.connect((host, port))
    print("connect successfully!")
except Exception:
    print('failed to connect!')
    sys.exit()
else:
    #Client computes key-value
    h = SM3(str(account) + str(password)).hash()
    k = h[:2]
    v = str((pow(int(h, 16), sk_client)) % n)

    #send (k, v) to the server
    addr = (host, port)
    client.sendto(k.encode('utf-8'), addr)
    client.sendto(v.encode('utf-8'), addr)

    # 从服务端接收H_ab与data set S
    H_ab,addr = client.recvfrom(1024 * 5)
    H_ab = int(H_ab.decode(),16)
    json_v,addr = client.recvfrom(1024 * 5)
    json_v = json_v.decode('utf-8')
    S = json.loads(json_v)
    print("S:",S)

    # 计算并检查H_b是否在S中
    H_b = (pow(H_ab,mod_inverse(sk_client,n-1)))%n
    tmp = 0
    for item in S:
        b = item
        if b == H_b:
            tmp = 1
    if tmp == 0:
        print('Account:',account,'Safe!')
    else:
        print('Account:',account,'Divulged')