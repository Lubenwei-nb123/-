import json
import socket
import sys
from os.path import commonprefix
from SM3 import *
from func_lib import *

#公私钥对
sk_client, pk = 15, 1376666

#客户端账号密码
account, password = 7777, 4396

#端口设置
host, port = '127.0.0.1', 1000
Address    = (host, port)
Client     = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


try:
    Client.connect((host, port))
    print("The Client connects successfully!")
except Exception:
    print('The Client failed to connect!')
    sys.exit()
else:
    #Client computes key-value
    h    = SM3(str(account) + str(password)).hash()
    k, v = h[:2], str((pow(int(h, 16), sk_client)) % pk)

    #send (k, v) to the server
    Client.sendto(k.encode('utf-8'), Address)
    Client.sendto(v.encode('utf-8'), Address)

    #receive H_ab and S from the server
    h__ab, Address  = Client.recvfrom(1024 * 5)
    h__ab           = int(h__ab.decode(), 16)
    json_S, Address = Client.recvfrom(1024 * 5)
    json_S          = json_S.decode('utf-8')
    S               = json.loads(json_S)
    print("data set S = ",S)

    #check whether h^b exists in S
    h__b = (pow(h__ab, mod_inverse(sk_client, pk - 1))) % pk
    cnt = 0
    for s in S:
        if s == h__b:
            cnt = 1
            print("user", account, "has been leaked!")
            break;
    if cnt == 0:
        print("user", account, "is safe!")