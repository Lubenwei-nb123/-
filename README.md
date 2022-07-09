# 创新创业实践大作业
SM3  
Project: implement the naïve birthday attack of reduced SM3  
Project: implement the Rho method of reduced SM3  
Project: implement length extension attack for SM3, SHA256, etc.  
Project: do your best to optimize SM3 implementation (software)：SM3_advanced.h, SM3_advanced.cpp  
2022.7.7：使用循环展开，使用inline关键字替代宏定义  
  
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

Bitcoin
Project: forge a signature to pretend that you are Satoshi

Bitcoin-public
*Project: send a tx on Bitcoin testnet, and parse the tx data down to every bit, better write script yourself
*Project: forge a signature to pretend that you are Satoshi

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
