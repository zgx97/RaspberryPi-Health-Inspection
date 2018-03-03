/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月24日 星期三 18时23分32秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server_opt.h"

#define MAXLINE 4096
#define DEBUG(args, str) { \
    printf(args, str); \
    fflush(stdout); \
}
#define _PRINT(args, str) { \
    printf(args, str); \
    fflush(stdout); \
}
int main(int argc, char *argv[]) {
    char buffer[MAXLINE];
    
    /* 首先要找到client的host和port, 将server端连接到client端 */
    char client_host[64];
    server_get_config_value("../config", "client_host", client_host);
    printf("Client Host :");  DEBUG("%s\n", client_host);
    int client_socket = server_socket_connect(MASTER_PORT, client_host);   
    /* 紧接着server端需要给Client端发送一个请求rqst, 然后client 
     * 端回复给server端相应的文件信息, 如果server端想要这个文件 
     * 那么server端会给client端回复一个ACK=1, 否则回复ACK=0
     * */
    int rqst = atoi(argv[1]);
    printf("Server Send Rqst :%d\n", rqst);
    server_send_msg(client_socket, rqst);
    server_recv_msg(client_socket, buffer, MAXLINE, 0);
    /* 根据传回来的buffer来判断ACK的状态为0还是1, 无论是0还是1
     * 都需要给client端一个回复ACK状态, 当返回一个ACK状态后 , 
     * server端马上进入监听状态
     * */
    int ACK_st = server_ACK_select(buffer);
    server_send_msg(client_socket, ACK_st);
    DEBUG("%s\n", "ACK send success!");

    if (ACK_st != 1) {
        printf("ACK status : %d\n", ACK_st);
        // sleep(30);
        close(client_socket);
        return 0;
    }
    
    int server_listen_socket = server_socket_create(8875);
    int client_data_sockte = -1;
    
    struct sockaddr_in data_addr;
    socklen_t data_addr_len = sizeof(data_addr);
    while (1) {
        /* if((client_data_sockte = server_socket_accept(server_listen_socket)) < 0) {
            continue;
        } */
        if ((client_data_sockte = accept(server_listen_socket, 
                                        (struct sockaddr *)(& data_addr),
                                         &data_addr_len)) < 0) {
            
            continue;
        }
        /* server接受client传过来的文件, 当文件传输成功之后, 
         * server端关闭与client的数据连接.
         * */
        DEBUG("%s\n", "\n>>>>>>>> Recv File Start! <<<<<<<<\n");
        server_recv_file(client_data_sockte, "../log/test.log");
        DEBUG("%s\n",   ">>>>>>>> Recv File End! <<<<<<<<\n");
        close(client_data_sockte);
        break;
    }
    close(client_socket);
    return 0;
}
