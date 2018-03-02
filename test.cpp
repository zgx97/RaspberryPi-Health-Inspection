/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年01月24日 星期三 21时00分34秒
 ************************************************************************/

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[]) {
    FILE *fn = fopen(argv[1], "w");
    char buffer[20] = "hello world";
    cout << buffer[2] << endl;
    fwrite(buffer, sizeof(char), 1, fn);
    fclose(fn);
    return 0;
}
