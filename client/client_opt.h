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


// 创建普通套接字
int client_nomal_socket_creat(int port);

// 创建具有监听状态的socket套接字
int client_socket_create(int port);

// client端确认连接请求
int client_socket_accept(int sock_listen);

// socket连接
int client_socket_connect(int port, char *host);

// client端发送msg
int client_send_msg(int sockfd, int rqst);

// client端接受msg
int client_recv_msg(int sockfd, char *buffer, int buffer_size, int _flag);

// client端回复msg
int _client_return_msg_aux(int rqst, char *log_dir_path);
int client_return_msg(int sockfd, int rqst);

// client端发送文件
int client_send_file(int sockfd, char *file_path);

// 得到配置文件
int client_get_config_value(const char *path_name, const char *key_name, char *value);

// 检查文件是否存在
int check_log_file(int reqcode, char *log_dir_path);

// 从一个字符串中查找相关信息
int client_find_str(const char *buffer, const char *key, char *value);

// 备份文件
int client_check_size(const char *filename, int size, const char *dirpath);
#endif
