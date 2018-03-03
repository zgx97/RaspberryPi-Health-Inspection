/* =================================================
 * REPLY CODE说明:
 *
 *  100 CPU
 *  101 NEN
 *  102 DISK
 *  103 PRO
 *  104 SYSINFO
 *  105 USERINFO
 *
 *  CLIENT REPLY CODE
 *  400 CPU      FILE NOT FOUND
 *  401 MEM      FILE NOT FOUND
 *  402 DISK     FILE NOT FOUND
 *  403 PRO      FILE NOT FOUND
 *  404 SYSINFO  FILE NOT FOUND
 *  405 USERINFO
 *
 *  CLIENT REPLY CODE
 *  200 CPU      FILE FOUND AND READY TO TRANSFER
 *  201 MEM      FILE FOUND AND READY TO TRANSFER
 *  202 DISK     FILE FOUND AND READY TO TRANSFER
 *  203 PRO      FILE FOUND AND READY TO TRANSFER
 *  204 SYSINFO  FILE FOUND AND READY TO TRANSFER
 *  205 USERINFO
 *
 *  300 CPU      NORAML OK
 *  301 MEM      NORAML OK
 *  302 DISK     NORAML OK
 *  303 PRO      NORAML OK
 *  304 SYSINFO  NORAML OK
 *  305 USERINFO
 * 
 *  500 CPU      FILE FINISHED
 *  501 MEM      FILE FINISHED
 *  502 DISK     FILE FINISHED
 *  503 PRO      FILE FINISHED
 *  504 SYSINFO  FILE FINISHED
 *  505 USERINFO
 * ================================================= */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "client_opt.h"

/* ==============================================
 * 函数: client_nomal_socket_creat
 * 参数: 传入开放端口
 * 功能: 创建普通套接字socket, 封装一层增加健壮性
 * ============================================== */
int client_nomal_socket_creat(int port) {
    int sockfd = -1;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("client_nomal_socket_creat() error!");
        exit(0);
    }
    return sockfd;
}

/* ==============================================
 * 函数: client_socket_create
 * 参数: 传入开放端口
 * 功能: 创建具有监听状态的client_socket
 * ============================================== */
int client_socket_create(int port){
	int sockfd;
	int yes = 1;
	struct sockaddr_in sock_addr;

	/* 创建套接字 */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error\n");
		return -1;
	}

	/* 设置本地套接字 */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		close(sockfd);
		perror("setsockopt() error\n");
		return -1;
	}

    /* 设置本地套接字地址 */
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port); //转化为网络字节序
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY); //0.0.0.0


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
 * 函数: client_socket_accept
 * 参数: 传入监听的套接字
 * 功能: 接受监听的套接字
 * ============================================== */
int client_socket_accept(int sock_listen) {
	int sockfd;
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr); 
	sockfd = accept(sock_listen, (struct sockaddr *) &client_addr, &len);
	if (sockfd < 0){
		perror("client_socket_accept() error");
		return -1;
	}
	return sockfd;
}

/* ==============================================
 * 函数: client_socket_connect
 * 参数: host & port
 * 功能: 传入端口和host之后建立tcp连接
 * ============================================== */
int client_socket_connect(int port, char *host) {
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
		perror("connect() error");
		return -1;
	}
	return sockfd;
}


/* ==============================================
 * 函数: check_log_file
 * 参数: 
 * 功能: 检查文件是否存在
 * ============================================== */
int check_log_file(int reqcode, char *log_dir_path) {
    char log_dir[1024];
    strcpy(log_dir, log_dir_path);
    switch (reqcode) {
        case 100 : strcat(log_dir, "cpu.log"); break;
        case 101 : strcat(log_dir, "mem.log"); break;
        case 102 : strcat(log_dir, "dsk.log"); break;
        case 103 : strcat(log_dir, "mpc.log"); break;
        case 104 : strcat(log_dir, "clc.log"); break;
        case 105 : strcat(log_dir, "usr.log"); break;
        default : {
            printf("check_log_file error!\n");
            return -1;
        }
    }
    printf("Check Log File() reqcode:%d, log_dir :%s\n", reqcode, log_dir);
    fflush(stdout);
    if ((access(log_dir, F_OK)) == -1) {
        printf("log_file non-existent!\n");
        return -1;
    }
    return 1;
}

/* =============================================
 * 函数: client_send_msg
 * 参数: 
 * 功能: 发送相关REPLY CODE
 * ============================================= */
int client_send_msg(int sockfd, int rqst) {
    char buffer[1024] = "Client send message=";
    int ind = strlen(buffer);
    buffer[ind++] = '0' + rqst / 100;
    buffer[ind++] = '0' + rqst / 10 % 10;
    buffer[ind++] = '0' + rqst % 10;
    buffer[ind] = '\0';
    printf("%s\n", buffer);
    fflush(stdout);
    if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
        printf("client_send_msg error!\n");
        return -1;
    }
    return 0;
}

/* =============================================
 * 函数: client_recv_msg
 * 参数: socket, buffer, buffersize,指定调用
 *       方式
 * 功能: 对recv函数进行一层封装,增加其健壮性, 它
 *       返回recv函数接受的实际数据长度
 * ============================================= */
int client_recv_msg(int sockfd, char *buffer, int buffer_size, int _flag) {
    int recv_len = -1;
    memset(buffer, 0, sizeof(char) * strlen(buffer));
    recv_len = recv(sockfd, buffer, buffer_size, _flag);
    printf("client_recv_msg :%s\n", buffer);
    fflush(stdout);
    if (recv_len <= 0) {
        printf("client_recv_msg() error!\n");
        // TODO : 我并不确定exit(0)会出现什么结果
        exit(0);
    }
    return recv_len;
}

/* =============================================
 * 函数: client_return_msg
 * 参数: 
 * 功能: 根据接受的SERVER REPLY CODE
 *       来返回一个REPLY CODE
 *       检查文件是否存在
 * ============================================= */
int _client_return_msg_aux_func(int rqst, const char *log_dir_path) {
    // 如果不存在文件则返回相应的REPLY CODE
    if (check_log_file(rqst, (char *)log_dir_path) == -1) {
        return rqst + 300; 
    }
    return rqst + 100;
}
int client_return_msg(int sockfd, int rqst) {
    int ret_code = 0;
    switch (rqst) {
        case 100 : ret_code = _client_return_msg_aux_func(100, "../log/"); break;
        case 101 : ret_code = _client_return_msg_aux_func(101, "../log/"); break;
        case 102 : ret_code = _client_return_msg_aux_func(102, "../log/"); break;
        case 103 : ret_code = _client_return_msg_aux_func(103, "../log/"); break;
        case 104 : ret_code = _client_return_msg_aux_func(104, "../log/"); break;
        case 105 : ret_code = _client_return_msg_aux_func(105, "../log/"); break;
        default : break;
    }
    client_send_msg(sockfd, ret_code);
    return ret_code;
}

/* =============================================
 * 函数: client_send_file
 * 参数:
 * 功能: client端发送文件
 * ============================================= */
int client_send_file(int sockfd, char *file_path) {
    printf("FilePath :%s\n", file_path);
    fflush(stdout);
    FILE *fp = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(file_path, "r");
    printf("fp :%p\n", fp);
    fflush(stdout);
    while ((read = getline(&line, &len, fp)) != -1) {
        printf("Client Send Msg:%s\n", line);
        printf("Client Send Msg length :%d\n", strlen(line));
        fflush(stdout);
        if (send(sockfd, line, strlen(line), 0) == -1) {
            printf("client_send_file error!\n");
            return -1;
        }
    }
    fclose(fp);
    return 0;
}


/* =============================================
 * 函数: client_get_config_value
 * 参数:
 * 功能: 得到config文件中的一些值
 * ============================================= */
int client_get_config_value(const char *path_name, const char *key_name, char *value) {
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

/* =============================================
 * 函数: client_find_str
 * 参数: 传入一个buffer和一个key
 * 功能: 在buffer中找到相关key的信息, 规定client
 *       和server在交流时, 其key和key相关的value
 *       的格式为key=value.
 * ============================================= */
int client_find_str(const char *buffer, const char *key, char *value) {
    char* st_ind = strstr(buffer, key);
    if (st_ind == NULL) {
        printf("client_find_str() error!\n");
        return -1;    
    }
    st_ind += strlen(key);
    int v_ind = 0;
    while (*st_ind != ' ') {
        *(value + (v_ind++)) = *st_ind;
        ++st_ind;
    }
    return 0;
}
