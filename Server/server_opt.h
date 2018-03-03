#ifndef COMMON_H
#define COMMON_H

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>		
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


#define MAX_SIZE 1024
#define MASTER_PORT 2048

struct request {	
    char arg[255];
	char code[7];
};

// 创建普通套接字, 封装一层
int server_nomal_socket_create(int port);

// 创建server端套接字，将其绑定到本地并且设置为监听状态
int server_socket_create(int port);

// server端接受一个连接
int server_socket_accept(int sock_listen);

// server端根据一个port和host进行连接
int server_socket_connect(int port, char *host);

// server端发送一个整形rqst给client端
int server_send_msg(int sockfd, int rqst);

// server端能接收一个msg
int server_recv_msg(int sockfd, char *buffer, int buffer_size, int _flag);

// 
int server_get_config_value(const char *path_name, const char *key_name, char *value);

// 对传入的buffer进行检查, 确定ACK的值
int server_ACK_select(char *buffer);

// 
int server_recv_file(int sockfd, char *file_path);

#endif
