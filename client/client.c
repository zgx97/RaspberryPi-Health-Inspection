/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年01月24日 星期三 15时29分05秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "client_opt.h"

#define MAXLINE 4096
#define _PRINT(arg, str) { \
    printf(arg, str); \
    fflush(stdout); \
}

int main(int argc, char *argv[]) {
    int server_socket;
    char recvline[MAXLINE];
    
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);

    server_socket = client_socket_create(MASTER_PORT);
    char host_ip[MAXLINE];
    client_get_config_value("./config", "master", host_ip);
    _PRINT("host_ip is %s\n", host_ip);
    // server_socket = client_socket_connect(MASTER_PORT, host_ip);
    
    /* client端一直监听server */
    int client_control_socket = -1;
    printf("Accept get ready!\n");
    while (1) {
        if ((client_control_socket = accept(server_socket, (struct sockaddr *)(& server_addr), &server_addr_len)) == -1) {
            // printf("client_control_socket value is %d\n", client_control_socket);
            continue;
        }
        printf("Client connect to Server!\n");
        fflush(stdout);
        /* 如果到达这里说明client端和server端已经建立起控制连接了
         * 这时候server端会传入一个rqst,  接受到一个recv_code并且
         * 检测相对应的文件是否存在,并返回给server一个code
         * */
        client_recv_msg(client_control_socket, recvline, MAXLINE, 0);
        _PRINT("recvline : %s\n", recvline);
        /* recv_code: client端从server端接受到的rqst_code 
         * retn_code: client端根据recv_code返回给server端的状态码
         * */
        char temp[30];
        client_find_str(recvline, "=", temp);
        int recv_code = atoi(temp);
        _PRINT("recv_code :%d\n", recv_code);
        int retn_code = client_return_msg(client_control_socket, recv_code);
        _PRINT("retn_code :%d\n", retn_code);
        /* 当client端发送给server端一个code后, server端先发送一个
         * ACK给Client端, 然后server端进入监听状态.  当client端接
         * 收到ACK=1的值后, 就确定要发送相关文件了,  这时候应该建
         * 立数据链接, 将相关日志文件发送给server端
         * */
        int recv_stat = client_recv_msg(client_control_socket, recvline, MAXLINE, 0);
        printf("recvline :%s\n", recvline);
        if (recv_stat <= 0) {
            printf("recv ACK error!\n");
            exit(0);
        }
        char ack_buf[64];
        client_find_str(recvline, "=", ack_buf);
        printf("ack_buf :%s\n", ack_buf);
        fflush(stdout);
        if (!((strlen(ack_buf) == 1) && (strcmp(ack_buf, "1") == 0))) {
            printf("ACK not equal 1!\n");
            exit(0);
        }
        
        /* 现在server已经做好接受信息的准备了, 现在应该是向server
         * 发送一个log文件了.
         * Step 1: 建立一个数据链接
         * Step 2: 找到log文件路径
         * Step 3: 发送log文件
         * Step 4: 关闭数据链接
         * */
        /* Step 1 */
        printf("Step 1!\n");
        int client_data_socket = -1;
        client_data_socket = client_socket_connect(8875, host_ip);
        /* Step 2 */
        printf("Step 2!\n");
        char log_path[MAXLINE] = "../log/";
        check_log_file(recv_code, log_path);
        switch(recv_code){
            case 100 : strcat(log_path, "cpu.log"); break;
            case 101 : strcat(log_path, "cpu.log"); break;
            case 102 : strcat(log_path, "cpu.log"); break;
            case 103 : strcat(log_path, "cpu.log"); break;
            case 104 : strcat(log_path, "cpu.log"); break;
        }
        /* Step 3 */
        printf("Step 3!\n");
        client_send_file(client_data_socket, log_path);
        /* Step 4 */
        printf("Step 4!\n");
        close(client_data_socket);
    }
    close(server_socket);
    return 0;
}

