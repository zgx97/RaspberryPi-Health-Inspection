/*************************************************************************
	> File Name: popen_test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年02月02日 星期五 14时27分07秒
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

int main() {
    FILE *fstream = NULL;
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int a = system("ls /etc");
    printf("a is : %d\n", a);
    if ((fstream = popen("ls /etc", "r")) == NULL) {
        fprintf(stderr, "cmd failed %s", strerror(errno));
        return -1;
    }
    while (fgets(buffer, sizeof(buffer), fstream) != NULL) {
        printf("%s\n", buffer);
    }
    int b = pclose(fstream);
    printf("b is : %d\n", b);
    system("echo \"hello\"");
    return 0;
}
