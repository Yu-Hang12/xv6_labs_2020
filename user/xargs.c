#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int k = 0;//读取输入流的长度

    int m = 0;
    char buf[32];//用来解析的缓冲区
    char* p = buf;
    char block[32];//用来输入的缓冲区

    int j = 0;//lineSplit的index
    char *lineSplit[32];//存储解析后的每行参数的字符串
    
    if(argc < 2) {
        printf("Usage: xargs <command> <parameter>\n");
        exit(0);
    }

    for(int i = 1; i < argc; i++) {
        lineSplit[j++] = argv[i];
    }

    while((k = read(0, block, sizeof(block))) > 0) {
        for(int l = 0; l < k; l++) {//检查读取的参数有没有换行或者空格
            if(block[l] == '\n') {
                buf[m] = 0;
                m = 0;
                lineSplit[j++] = p;
                p = buf;
                lineSplit[j] = 0;
                j = argc - 1;
                if(fork() == 0) {
                    exec(argv[1], lineSplit);
                }
                wait(0);
            } else if (block[l] == ' ') {
                buf[m++] = '0';
                lineSplit[j++] = p;
                p = &buf[m];
            } else {
                buf[m++] = block[l];
            }
        }
    }
    exit(0);
}