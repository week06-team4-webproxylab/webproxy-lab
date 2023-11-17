#include "csapp.h"

int main(int argc, char **argv)
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = argv[2];
                // Open Client File Descriptor
    clientfd = Open_clientfd(host, port); // 지정된 호스트와 포트에 대한 클라이언트 소켓을 연다.
    // Robust I/O Read Initialize Buffer
    Rio_readinitb(&rio, clientfd); // Robust I/O (RIO) 패키지를 초기화.
    
            // File Gets, 표준 C 라이브러리 함수
    while (Fgets(buf, MAXLINE, stdin) != NULL) // 파일 or 스트림에서 한 줄을 읽어옴. 
    {   //  Robust I/O Write N Bytes
        Rio_writen(clientfd, buf, strlen(buf)); // 지정된 바이트 수 만큼 데이터를 안정적으로 쓰기 위한 함수.
        // Robust I/O Read Line Buffer
        Rio_readlineb(&rio, buf, MAXLINE); // 한 줄의 데이터를 안정적으로 읽기 위한 함수.
        Fputs(buf, stdout);
    }

    //  Close File Descriptor, 표준 C 라이브러리 함수
    Close(clientfd); // 파일 or 소켓 디스크립터를 닫는 함수
    exit(0);
}

/*
* "Robust" 패키지는 네트워크 프로그래밍과 시스템 프로그래밍에서 
* 발생할 수 있는 다양한 오류 상황을 더 안정적으로 처리할 수 있도록 
* 설계된 라이브러리나 함수의 모음을 말합니다. 
* 이러한 패키지는 특히 I/O(입출력) 작업을 수행할 때 
* 발생할 수 있는 예외 상황들을 안전하게 처리하는 데 중점을 둡니다.
*/