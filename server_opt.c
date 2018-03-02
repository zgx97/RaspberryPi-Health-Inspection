
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "server_opt.h"

#define _PRINT(arg, str) { \
    printf(arg, str); \
    fflush(stdout); \
}

/* ==============================================
 * 函数: server_nomal_socket_create
 * 参数: 传入开放端口
 * 功能: 创建普通状态的server_socket
 * ============================================== */
int server_nomal_socket_create(int port) {
    int sockfd = -1;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("server_nomal_socket_create() error!");
        exit(0);
    }
    return sockfd;
}

/* ==============================================
 * 函数: server_socket_create
 * 参数: 传入开放端口
 * 功能: 创建具有监听状态的server_socket
 * ============================================== */
int server_socket_create(int port){
	int sockfd;
	int yes = 1;
	struct sockaddr_in sock_addr;

	/* 创建套接字 */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error\n");
		return -1;
	}
	
    /* 设置本地套接字地址 */
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);               // 转化为网络字节序
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 0.0.0.0

	/* 设置本地套接字 */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		close(sockfd);
		perror("setsockopt() error\n");
		return -1;
	}

    
	/* 绑定本地套接字到套接字 */
	if (bind(sockfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
		close(sockfd);
		perror("bind() error!\n");
		return -1;
	}

	/* 将套接字设置为监听状态 */
	if (listen(sockfd, 20) < 0) {
		close(sockfd);
		perror("listen() error");
		return -1;
	}
	return sockfd;
}

/* ==============================================
 * 函数: server_socket_accept
 * 参数: 传入监听端口
 * 功能: 接受一个connect
 * ============================================== */
int server_socket_accept(int sock_listen) {
	int sockfd;
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr); 
	sockfd = accept(sock_listen, (struct sockaddr *) &client_addr, &len);
	if (sockfd < 0){
		perror("server_socket_accept() error");
		return -1;
	}
	return sockfd;
}

/* ==============================================
 * 函数: server_socket_connect
 * 参数: 传入client端的port和host
 * 功能: 将server端连接到client端
 * ============================================== */
int server_socket_connect(int port, char *host) {
	int sockfd;
	struct sockaddr_in dest_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error");
		return -1;
	}

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = inet_addr(host);

	if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
		perror("server_socket_connect() error");
		return -1;
	}
	return sockfd;
}

/* ==============================================
 * 函数: server_send_msg
 * 参数: 传入client端的socket以及send的值
 * 功能: server端发送一个整型的rqst给client端
 * ============================================== */
int server_send_msg(int sockfd, int rqst) {
    char buffer[64] = "Server Send Message=";
    char temp[64];
    sprintf(temp, "%d ", rqst);
    strcat(buffer, temp);
    _PRINT("%s\n", buffer);
    if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
        printf("server_send_request error!\n");
        return -1;
    }
    printf("server send massage : %s\n", buffer);
    fflush(stdout);
    return 0;
}

/* ==============================================
 * 函数: server_recv_msg
 * 参数: 传入一个socket和一个rqst code
 * 功能: 向目标socket发送一个rqst code,  每次
 *       先清空buffer, 防止buffer中有残留数据
 * ============================================== */
int server_recv_msg(int sockfd, char *buffer, int buffer_size, int _flag) {
    printf("in recv msg!\n");
    fflush(stdout);
    int recv_len = -1;
    memset(buffer, 0, 4096);
    if ((recv_len = recv(sockfd, buffer, buffer_size, _flag)) < 0) {
        printf("server_recv_msg() error!\n");
        return 0;
    }
    printf("recv_len : %d\n", recv_len);
    fflush(stdout);
    _PRINT("recv_len : %d\n", recv_len);
    _PRINT("recv_buffer : %s\n", buffer);
    return recv_len;
}

/* ==============================================
 * 函数: server_get_config_value
 * 参数: config文件路径名,config中的关键字
 *       将答案带出的value指针
 * 功能: 得到config文件中相关关键字的信息
 * ============================================== */
int server_get_config_value(const char *path_name, const char *key_name, char *value) {
    FILE *fp = NULL;
    char *line = NULL, *substr = NULL;
    size_t len = 0, tmplen = 0;
    ssize_t read = 0;
    
    if (key_name == NULL || value == NULL) {
        printf("paramer is invaild!\n");
        return -1;
    }

    fp = fopen(path_name, "r");
    if (fp == NULL) {
        printf("Open config file error!\n");
        return -1;
    }
    
    while ((read = getline(&line, &len, fp)) != -1) {
        substr = strstr(line, key_name);
        if (substr == NULL) {
            continue;
        } else {
            tmplen = strlen(key_name);
            if (line[tmplen] == '=') {
                strncpy(value, &line[tmplen + 1], len - tmplen + 1);
                tmplen = strlen(value);
                *(value + tmplen - 1) = '\0';
                break;
            } else {
                printf("Maybe there is something wrong with config file\n");
                continue;
            }
        }
    }

    if (substr == NULL) {
        printf("%s not found in config file\n", key_name);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 1;
}

/* ==============================================
 * 函数: server_ACK_select
 * 参数: 传入一个字符串buffer 
 * 功能: 检查buffer, 然后返回一个ACK状态
 * ============================================== */
int server_ACK_select(char *buffer) {
    int ACK_st = -1;
    char *tstr = strstr(buffer, "RECV_CODE=") + strlen("RECV_CODE=");
    printf("tstr=%s*\n", tstr);
    int retval = (tstr[0] - '0') * 100 + (tstr[1] - '0') * 10 + (tstr[2] - '0');
    printf("retval is : %d\n", retval);
    fflush(stdout);
    switch (retval / 100) {
        case 2: ACK_st = 1; break;
        case 3: ACK_st = 1; break;
        default: {
            ACK_st = 0;
            printf("server_ACK_select() error!\n");
            // exit(0);
        }
    }
    return ACK_st;
}

/* ==============================================
 * 函数: server_recv_file
 * 参数: sockfd & file_path
 * 功能: 从sockdf中接受一行信息存放到file_path中
 * ============================================== */
int server_recv_file(int sockfd, char *file_path) {
    FILE *file_fd = fopen(file_path, "a+");
    
    char buffer[4096];
    int recv_len = -1;
    _PRINT("In Server Recv File!%d", 1);
    while (server_recv_msg(sockfd, buffer, 4096, 0) > 0) {
        fwrite(buffer, sizeof(char), 4096, file_fd);
        _PRINT("recv buffer : %s\n", buffer);
    }   
    printf("recv loop end!\n");
    fclose(file_fd);
    return 0;
} 

/*
int server_ACK_select(char *buffer) {
    char *ack_st = strstr(buffer, "ACK");
    if (ack_st == NULL) {
        printf("sever_ACK_select() error!\n");
        exit(0);
    }
    ack_st += strlen("ACK");
    char tstr[16];
    int ind = 0;
    while (*ack_st != ' ') {
        tstr[ind++] = *ack_st; 
        ++ack_st;
    }
    int retval = atoi(tstr);
    if (retval < 0 || retval > 1) {
        printf("sever_ACK_select() error!\n");
        exit(0);
    }
    return retval;
}
*/

