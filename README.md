# 创新创业实践大作业

## 小组成员

| 成员   | 学号         | Github账号     |
| ------ | ------------ | -------------- |
| 翁梓诚 | 202000460122 | Lubenwei-nb123 |

## 任务完成情况

| 任务                                                         | 是否完成 |
| ------------------------------------------------------------ | -------- |
| implement the naïve birthday attack of reduced SM3           | ✔        |
| implement the Rho method of reduced SM3                      | ✔        |
| implement length extension attack for SM3, SHA256, etc.      | ✔        |
| do your best to optimize SM3 implementation (software)       | ✔        |
| Impl Merkle Tree following RFC6962                           | ✔        |
| Try to Implement this scheme                                 |          |
| report on the application of this deduce technique in Ethereum with ECDSA | ✔        |
| Impl Merkle Tree following RFC6962                           | ✔        |
| impl sm2 with RFC6979                                        |          |
| verify the above pitfalls with proof-of-concept code         | ✔        |
| Implement the above ECMH scheme                              | ✔        |
| Implement a PGP scheme with SM2                              |          |
| implement sm2 2P sign with real network communication        | ✔         |
| PoC impl of the scheme, or do implement analysis by Google   | ✔         |
| implement sm2 2P decrypt with real network communication     | ✔         |
| send a tx on Bitcoin testnet, and parse the tx data down to every bit, better write script yourself |          |
| forge a signature to pretend that you are Satoshi            | ✔        |
| research report on MP                                        | ✔        |
| Find a key with hash value “sdu_cst_20220610” under a message composed of your name followed by your student ID. For example, “San Zhan 202000460001” |          |
| Find a 64-byte message under some k fulfilling that their hash value is symmetrical |          |
| Real world zk                                                |          |



## SM3  

### Project: implement the naïve birthday attack of reduced SM3  

#### 运行方法

下载SM3_advanced后，打开SM3_advanced.cpp直接运行即可

#### 结论

最终找到了16bits的碰撞.

![image-20220730150634148](https://user-images.githubusercontent.com/78082874/181903157-59b03eba-0686-45a0-8726-bdf887b5ad81.png)

### Project: implement the Rho method of reduced SM3  

#### 运行方法

下载SM3_advanced后，打开SM3_advanced.cpp直接运行即可

#### 结论

最终找到了256bits的碰撞.

初始明文为$M = M_1||M_2||M_3||M_4||M_5||M_6 = 0x01||0x02||0x03||0x04||0x05||0x06$

迭代函数为$M_1||M_2||M_3||M4||M_5||M_6 \leftarrow M_1+2||M_2+2||M_3+2||M_4+2||M_5+2||M_6+2$

分别进行前8、16、128、256bits的碰撞，运行结果如下：

![image-20220730115102064](https://user-images.githubusercontent.com/78082874/181903150-b4bf2831-2b69-4265-8bdd-457639ea4886.png)



### Project: implement length extension attack for SM3, SHA256, etc.  
#### 运行方法
下载SM3_advanced后，打开SM3_advanced.cpp直接运行即可  

#### 结论
2022.7.14  
以字符串"SDU"为样例，输出如下：  
![image](https://user-images.githubusercontent.com/78082874/179014712-498c4684-43c8-471f-b8f0-201a00b6728f.png)

### Project: do your best to optimize SM3 implementation (software)  

#### 运行方法

下载SM3_advanced后，打开SM3_advanced.cpp直接运行即可

#### 结论

2022.7.7  

1. 使用循环展开，让循环CPE逼近吞吐量界限。  
2. 使用inline关键字替代宏定义，使得算法鲁棒性更强.(详见Effective C++)  
3. 尽可能使用三目运算符代替if{}else{}，即使用数据传送来代替条件转移，提升效率.(详见CSAPP)  
以下是运行截图  
![image](https://user-images.githubusercontent.com/78082874/182010013-5f2eec49-4f41-420c-b2a7-fe1f96548366.png)

### Project: Impl Merkle Tree following RFC6962：Merkel_Tree.go  

#### 运行方法

打开Merkle_Tree.go后直接运行即可

#### 结论

基于Go语言的Merkle树实现，其中附带了一个样例：  
![image](https://user-images.githubusercontent.com/78082874/178011264-26870bc4-36e0-4d67-9d12-55f25d47517a.png)  
输出结果如下：  
![image](https://user-images.githubusercontent.com/78082874/178011604-a20de593-5586-4b99-a1b2-518ba26bd88c.png)  

### Project: Try to Implement this scheme   

## SM2  

### *Project: report on the application of this deduce technique in Ethereum with ECDSA：SM2_recover_pk 

#### 运行方法

将deduce.py和func_lib.py放在同一文件夹下后运行deduce.py即可

#### 结论

明文以字符串"SDU"为样例，输出如下：  
![image](https://user-images.githubusercontent.com/78082874/179349603-ff2f67ba-544b-4859-bfe4-29eb5e5cb62f.png)  

### *Project: impl sm2 with RFC6979：SM2_impl  


### *Project: verify the above pitfalls with proof-of-concept code  
#### 运行方法

下载pitfalls后，直接运行pitfalls.py即可.

#### 结论

按照课件上的方法进行攻击即可，运行结果如下：

![image](https://user-images.githubusercontent.com/78082874/181933540-195f803d-6707-4f4d-b798-a176f58bd628.png)  

参考链接：[(14条消息) SM3算法 (python)_Love is Zero的博客-CSDN博客_sm3python](https://blog.csdn.net/qq_43752749/article/details/123579647)

### *Project: Implement the above ECMH scheme 
#### 运行方法

下载ECMH后，直接运行ECMH.py即可

#### 结论

多集合哈希有以下优点：

- 非常高效
- 快速验证比特币所有权

运行结果如下：
![image](https://user-images.githubusercontent.com/78082874/182007745-c2069ef7-1703-4641-8ba5-97ccbf18169b.png)  
参考链接：[[bitcoin-dev\] Rolling UTXO set hashes (linuxfoundation.org)](https://lists.linuxfoundation.org/pipermail/bitcoin-dev/2017-May/014337.html)  
### *Project: Implement a PGP scheme with SM2  

### *Project: implement sm2 2P sign with real network communication  
#### 运行方法

下载2 parties sign后，先运行P2.py再运行P1.py即可.

#### 结论

按照PPT上的方法编写代码，运行截图如下  
![image](https://user-images.githubusercontent.com/78082874/182011560-4934f7dd-9957-4c35-9991-5e5eb0d7a0db.png)  
![image](https://user-images.githubusercontent.com/78082874/182011567-01fdc144-9a0a-4dba-9cc4-73d3e81b58cf.png)  
  
### *Project: PoC impl of the scheme, or do implement analysis by Google  
#### 运行方法

下载PoC后，先运行server.py，再运行client.py即可.

#### 结论

根据PPT上编写代码即可，运行截图如下
![image](https://user-images.githubusercontent.com/78082874/182009786-eba65319-b75c-4634-8b33-e7147df0e70b.png)
![image](https://user-images.githubusercontent.com/78082874/182009789-d8ea86e1-fdff-4664-9aab-e1193bbad199.png)

### *Project: implement sm2 2P decrypt with real network communication  
#### 运行方法

下载2 parties dec后，先运行P2.py，再运行P1.py即可.

#### 结论

根据课件编写代码即可，运行截图如下
![image](https://user-images.githubusercontent.com/78082874/182013102-e280d49e-015c-4375-856a-0e19a7637303.png)  
![image](https://user-images.githubusercontent.com/78082874/182013106-1ac1d133-3468-4d3d-af0e-1d1f3d5f66c8.png)  
  
参考链接：https://blog.csdn.net/HORHEART/article/details/120123657
## Bitcoin-public  

### *Project: send a tx on Bitcoin testnet, and parse the tx data down to every bit, better write script yourself  

### *Project: forge a signature to pretend that you are Satoshi  

#### 运行方法

下载forge Satoshi's signature后直接运行forge sign.py即可

#### 结论

通过Google搜索，在https://cryptome.org/2015/11/satoshi-nakamoto-public-key.htm 上找到了中本聪的公钥：  
-----BEGIN PGP PUBLIC KEY BLOCK-----  
Version: GnuPG v1.4.7 (MingW32)  

mQGiBEkJ+qcRBADKDTcZlYDRtP1Q7/ShuzBJzUh9hoVVowogf2W07U6G9BqKW24r  
piOxYmErjMFfvNtozNk+33cd/sq3gi05O1IMmZzg2rbF4ne5t3iplXnNuzNh+j+6  
VxxA16GPhBRprvnng8r9GYALLUpo9Xk17KE429YYKFgVvtTPtEGUlpO1EwCg7FmW  
dBbRp4mn5GfxQNT1hzp9WgkD/3pZ0cB5m4enzfylOHXmRfJKBMF02ZDnsY1GqeHv  
/LjkhCusTp2qz4thLycYOFKGmAddpVnMsE/TYZLgpsxjrJsrEPNSdoXk3IgEStow  
mXjTfr9xNOrB20Qk0ZOO1mipOWMgse4PmIu02X24OapWtyhdHsX3oBLcwDdke8aE  
gAh8A/sHlK7fL1Bi8rFzx6hb+2yIlD/fazMBVZUe0r2uo7ldqEz5+GeEiBFignd5  
HHhqjJw8rUJkfeZBoTKYlDKo7XDrTRxfyzNuZZPxBLTj+keY8WgYhQ5MWsSC2MX7  
FZHaJddYa0pzUmFZmQh0ydulVUQnLKzRSunsjGOnmxiWBZwb6bQjU2F0b3NoaSBO  
YWthbW90byA8c2F0b3NoaW5AZ214LmNvbT6IYAQTEQIAIAUCSQn6pwIbAwYLCQgH  
AwIEFQIIAwQWAgMBAh4BAheAAAoJEBjAnoZeyUihXGMAnjiWJ0fvmSgSM3o6Tu3q  
RME9GN7QAKCGrFw9SUD0e9/YDcqhX1aPMrYue7kCDQRJCfqnEAgA9OTCjLa6Sj7t  
dZcQxNufsDSCSB+yznIGzFGXXpJk7GgKmX3H9Zl4E6zJTQGXL2GAV4klkSfNtvgs  
SGJKqCnebuZVwutyq1vXRNVFPQFvLVVo2jJCBHWjb03fmXmavIUtRCHoc8xgVJMQ  
LrwvS943GgsqSbdoKZWdTnfnEq+UaGo+Qfv66NpT3Yl0CXUiNBITZOJcJdjHDTBO  
XRqomX2WSguv+btYdhQGGQiaEx73XMftXNCxbOpqwsODQns7xTcl2ENru9BNIQME  
I7L9FYBQUiKHm1k6RrBy1as8XElS2jEos7GAmlfF1wShFUX+NF1VOPdbN3ZdFoWq  
sUjKk+QbrwADBQgA9DiD4+uuRhwk2B1TmtrXnwwhcdkE7ZbLHjxBfCsLPAZiPh8c  
ICfV3S418i4H1YCz2ItcnC8KAPoS6mipyS28AU1B7zJYPODBn8E7aPSPzHJfudMK  
MqiCHljVJrE23xsKTC0sIhhSKcr2G+6ARoG5lwuoqJqEyDrblVQQFpVxBNPHSTqu  
O5PoLXQc7PKgC5SyQuZbEALEkItl2SL2yBRRGOlVJLnvZ6eaovkAlgsbGdlieOr0  
UwWuJCwzZuBDruMYAfyQBvYfXZun3Zm84rW7Jclp18mXITwGCVHg/P5n7QMbBfZQ  
A25ymkuj636Nqh+c4zRnSINfyrDcID7AcqEb6IhJBBgRAgAJBQJJCfqnAhsMAAoJ  
EBjAnoZeyUihPrcAniVWl5M44RuGctJe+IMNX4eVkC08AJ9v7cXsp5uDdQNo8q3R  
8RHwN4Gk8w==  
=3FTe  
-----END PGP PUBLIC KEY BLOCK-----  

但是由于我不太清楚如何处理PGP格式的公钥，故无法在我的代码上真正的伪造中本聪的信息，只能写出对应伪造过程的代码，并验证其正确性.

![image-20220730164807543](https://user-images.githubusercontent.com/78082874/181903127-6ab45a46-1522-426a-816c-d4ee22db4c8e.png)

## Eth-public  

### Project: research report on MP  
#### 运行方法

打开Research report on MP中的README即可.

#### 结论

略

## Real world cryptanalyses  

### Project: Find a key with hash value “sdu_cst_20220610” under a message composed of your name followed by your student ID. For example, “San Zhan 202000460001”.  

### Project: Find a 64-byte message under some k fulfilling that their hash value is symmetrical  

## Real world zk  

### Project Idea   

1. Write a circuit to prove that your CET6 grade is larger than 425  
a. Your grade info is like (cn_id, grade, year, sig_by_moe)，These grades are published as commitments onchain by MoE.  
b. When you got an interview from an employer, you can prove to them that you have passed the exam without letting them know the exact grade.  
2. The commitment scheme used by MoE is SHA256-based.  
a. commit = SHA256(cn_id, grade, year, sig_by_moe, r)
