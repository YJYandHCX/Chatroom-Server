# Chatroom-Server
是在学习《Linux服务器编程》做的小项目
修改书第9章的chatroom服务器代码
C/S模型。I/O epoll复用（原书poll）。
在Linux 16.04 版本开发
使用方法：
g++ -o Server.h Server.cpp main.cpp
采用telnet 模拟客户端程序
telnet 127.0.0.1 12345
连接服务器就可以愉快玩耍了
