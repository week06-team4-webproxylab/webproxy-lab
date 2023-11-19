#include "csapp.h"

// 반복적 echo 서버 메인 루틴

void echo(int connfd);

int main(int argc, char **argv)
{   
    int listenfd, connfd; // 리스닝 소켓, 클라이언트 소켓
    socklen_t clientlen;
    struct sockaddr_storage clientaddr; // 클라이언트의 주소 정보 저장 /* Enough space for any address */
    char client_hostname[MAXLINE], client_port[MAXLINE];

    if (argc != 2)  // argc 가 2가 아닐 경우 사용 방법을 출력하고 프로그램 종료
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]); // 주어진 포트 번호에서 들어오는 연결 요쳥을 기다리는 리스닝 소켓을 생성
    
    while (1) // 무한 루프를 통해 서버는 연결 요청을 계속 기다린다.
    {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); // 새로운 클라이언트의 연결 요청을 받아들이고, 해당 클라이언트와의 통신을 위한 새로운 소켓(connfd)을 생성
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE,
                    client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        echo(connfd);
        Close(connfd);
    }
    exit(0);
}