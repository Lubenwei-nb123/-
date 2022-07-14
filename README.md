# 创新创业实践大作业
SM3  
Project: implement the naïve birthday attack of reduced SM3  
Project: implement the Rho method of reduced SM3  
Project: implement length extension attack for SM3, SHA256, etc.：SM3_advanced.cpp的main函数  
2022.7.14  
以字符串"SDU"为样例，输出如下：  
![image](https://user-images.githubusercontent.com/78082874/179010114-5f09e7b5-8736-4e4a-ab8d-ddf02611d054.png)
Project: do your best to optimize SM3 implementation (software)：SM3_advanced.h, SM3_advanced.cpp  
2022.7.7  
1.使用循环展开，让循环CPE逼近吞吐量界限。  
2.使用inline关键字替代宏定义，使得算法鲁棒性更强.(详见Effective C++)  
3.尽可能使用三目运算符代替if{}else{}，即使用数据传送来代替条件转移，提升效率.(详见CSAPP)  
  
Project: Impl Merkle Tree following RFC6962：Merkel_Tree.go  
基于Go语言的Merkle树实现，其中附带了一个样例：  
![image](https://user-images.githubusercontent.com/78082874/178011264-26870bc4-36e0-4d67-9d12-55f25d47517a.png)  
输出结果如下：  
![image](https://user-images.githubusercontent.com/78082874/178011604-a20de593-5586-4b99-a1b2-518ba26bd88c.png)  

Project: Try to Implement this scheme   
  
SM2  
*Project: report on the application of this deduce technique in Ethereum with ECDSA  
*Project: impl sm2 with RFC6979：SM2_impl  
基于开源MIRACL库的实现  
  
*Project: verify the above pitfalls with proof-of-concept code  
*Project: Implement the above ECMH scheme  
*Project: Implement a PGP scheme with SM2  
*Project: implement sm2 2P sign with real network communication  
*Project: PoC impl of the scheme, or do implement analysis by Google  
*Project: implement sm2 2P decrypt with real network communication  
  
Bitcoin-public  
*Project: send a tx on Bitcoin testnet, and parse the tx data down to every bit, better write script yourself  
*Project: forge a signature to pretend that you are Satoshi  
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
  
Eth-public  
Project: research report on MP  
  
Real world cryptanalyses  

Project: Find a key with hash value “sdu_cst_20220610” under a message composed of your name followed by your student ID. For example, “San Zhan 202000460001”.  
Project: Find a 64-byte message under some k fulfilling that their hash value is symmetrical  
  
Real world zk  
Project Idea   
1. Write a circuit to prove that your CET6 grade is larger than 425  
a. Your grade info is like (cn_id, grade, year, sig_by_moe)，These grades are published as commitments onchain by MoE.  
b. When you got an interview from an employer, you can prove to them that you have passed the exam without letting them know the exact grade.  
2. The commitment scheme used by MoE is SHA256-based.  
a. commit = SHA256(cn_id, grade, year, sig_by_moe, r)
