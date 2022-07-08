# 创新创业实践大作业
SM3  
Project: implement the naïve birthday attack of reduced SM3  
Project: implement the Rho method of reduced SM3  
Project: implement length extension attack for SM3, SHA256, etc.  
Project: do your best to optimize SM3 implementation (software)：SM3_advanced.h, SM3_advanced.cpp  
Project: Impl Merkle Tree following RFC6962：Merkel_Tree.go  
基于Go语言的Merkle树实现，其中附带了一个样例：  
![image](https://user-images.githubusercontent.com/78082874/178011264-26870bc4-36e0-4d67-9d12-55f25d47517a.png)
输出结果如下：  
0xc000110630  
left[0xc0000660a0],right[0xc0000660c8],data(8e22e9687fa7b93f5cc2a12bf74b97fd1f36cf65f4a494d4d9ec472c26e9f18a)  
0xc0000660a0                                                                                                    
left[0xc00006a140],right[0xc00006a168],data(eaf53c5fdeeba7f3da9132b8d354cfed552673e3e8f35ec8cf31173b310fb965)  
0xc00006a140                                                                                                   
left[0x0],right[0x0],data(2f3f831ec65dcc4603ff46245be5232a86cd4ffcc83740de411de119b7e38279)                    
0xc00006a168                                                                                                   
left[0x0],right[0x0],data(fa51fd49abf67705d6a35d18218c115ff5633aec1f9ebfdc9d5d4956416f57f6)                    
0xc0000660c8                                                                                                   
left[0xc00006a190],right[0xc00006a1b8],data(9c617d5a91a9d112edb217b89e02a2f093f5b73a9c86e4e71e79b836351eea01)  
0xc00006a190                                                                                                   
left[0x0],right[0x0],data(46a0552f009f9f549024cf9eaed77b71558dc10d262ab1a94b5a7b89eea99025)  
0xc00006a1b8  
left[0x0],right[0x0],data(adba6c0ec8a8d89efb03de642427a09302fa4f7474989ecf33fd545a30d2ff5b)  

Project: Try to Implement this scheme   
