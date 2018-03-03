# 井号开头的行是一个注释
# 设置 C 语言的编译器
CC = gcc

# -g 增加调试信息
# -Wall 打开大部分警告信息
CFLAGS = -g -Wall

# 整理一下 main 依赖哪些目标文件
MAINOBJS_CLIENT = client.o client_opt.o
MAINOBJS_SERVER = server.o server_opt.o

.PHONY: clean

client: $(MAINOBJS_CLIENT)
	$(CC) $(CFLAGS) -o ct $(MAINOBJS_CLIENT)

server: $(MAINOBJS_SERVER)
	$(CC) $(CFLAGS) -o sv $(MAINOBJS_SERVER)

# CLIENT
client_opt.o: client_opt.c client_opt.h
	$(CC) $(CFLAGS) -c -o client_opt.o client_opt.c 
client.o: client.c client_opt.h
	$(CC) $(CFLAGS) -c -o client.o client.c

# SERVER 
server_opt.o: server_opt.c server_opt.h
	$(CC) $(CFLAGS) -c -o server_opt.o server_opt.c
server.o: server.c server_opt.h
	$(CC) $(CFLAGS) -c -o server.o server.c

clean:
	rm -rf $(MAINOBJS_CLIENT) $(MAINOBJS_SERVER)
	rm ct sv
