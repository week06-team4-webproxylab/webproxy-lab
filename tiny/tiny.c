/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the
 *     GET method to serve static and dynamic content.
 *
 * Updated 11/2019 droh
 *   - Fixed sprintf() aliasing issue in serve_static(), and clienterror().
 */
#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg,
                 char *longmsg);

int main(int argc, char **argv)
{
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  /* Check command line args */
  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  listenfd = Open_listenfd(argv[1]);
  while (1)
  {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr,
                    &clientlen); // line:netp:tiny:accept
    Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE,
                0);
    printf("Accepted connection from (%s, %s)\n", hostname, port);
    doit(connfd);  // line:netp:tiny:doit
    Close(connfd); // line:netp:tiny:close
  }
}

void doit(int fd)
{
  int is_static;
  struct stat sbuf;
  char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  char filename[MAXLINE], cgiargs[MAXLINE];
  rio_t rio;

  // 버퍼(rio) 주소, 식별자fd와 연결
  Rio_readinitb(&rio, fd);
  //
  Rio_readlineb(&rio, buf, MAXLINE);
  printf("Request header:\n");
  printf("%s", buf); // 그대로 출력?
  sscanf(buf, "%s %s %s", method, uri, version);
  if (strcasecmp(method, "GET")) //?
  {
    clienterror(fd, method, "501", "Not implemented",
                "Tiny does not implement this method");
    return;
  }
  read_requesthdrs(&rio);

  is_static = parse_uri(uri, filename, cgiargs);
  if (stat(filename, &sbuf) < 0) //?
  {
    clienterror(fd, filename, "404", "Not found",
                "Tiny couldn't find this file");
    return;
  }

  if (is_static)
  { // 파일 종류 확인해서 읽기 권한을 확인하는 함수
    if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))
    { /* Read by owner.  */
      clienterror(fd, filename, "403", "Forbidden",
                  "Tiny couldn't read the file");
      return;
    }
    serve_static(fd, filename, sbuf.st_size);
  }
  else
  { /* Execute by owner.  */
    if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))
    {
      clienterror(fd, filename, "403", "Forbidden",
                  "Tiny couldn't run the CGI program");
      return;
    }
    serve_dynamic(fd, filename, cgiargs);
  }
}

void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
  char buf[MAXLINE], body[MAXBUF];
  /* Build the HTTP response body */
  sprintf(body, "<html><title>Tiny Error</title>");
  sprintf(body, "%s<body bgcolor="
                "ffffff"
                ">\r\n",
          body);
  sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
  sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
  sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);
  /* Print the HTTP response */
  sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-type: text/html\r\n");
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
  Rio_writen(fd, buf, strlen(buf));
  Rio_writen(fd, body, strlen(body));
}

void read_requesthdrs(rio_t *rp)
{
  char buf[MAXLINE];

  Rio_readlineb(rp, buf, MAXLINE);
  while (strcmp(buf, "\r\n")) // 대소문자 구분없이 buf 읽는다
  {
    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
  }
  return;
}

int parse_uri(char *uri, char *filename, char *cgiargs)
{
  char *ptr;

  if (!strstr(uri, "cgi-bin"))
  {                        // 정적인 컨텐츠(리눅스 디렉토리 지정)
    strcpy(cgiargs, "");   // 디렉토리 경로 지우기
    strcpy(filename, "."); // 파일이름을 `./index.html`형식으로 만든다.
    strcat(filename, uri); // 두 개의 문자열을 연결(Concatenate)하는 데 사용

    if (uri[strlen(uri) - 1] == '/') //?
      strcat(filename, "home.html"); // www.aol.com/home.html
    return 1;
  }
  else
  {                        // 동적인 컨텐츠
    ptr = index(uri, '?'); // 포인터를 `?`로 설정
    if (ptr)
    {                           //             1000&213을 복사해서 cgiargs에 대입
      strcpy(cgiargs, ptr + 1); // cgi-bin/adder?1000&213 HTTP
      *ptr = '\0';              // `?1000&213 HTTP` 삭제
    }                           // uri는 GET/cgi-bin/adder 된다.
    else
    {
      strcpy(cgiargs, "");
    }

    strcpy(filename, ".");
    strcat(filename, uri); // ./cgi-bin/adder
    return 0;
  }
}

void serve_static(int fd, char *filename, int filesize)
{
  int srcfd;
  char *srcp, filetype[MAXLINE], buf[MAXBUF];

  /* Send response headers to client */
  get_filetype(filename, filetype);
  sprintf(buf, "HTTP/1.0 200 OK\r\n"); // `HTTP/1.0 200 OK\r\n` buf에 넣기 
  sprintf(buf, "%sServer: Tiny Web Server\r\n", buf); // `HTTP/1.0 200 OK\r\nServer: Tiny Web Server\r\n` 를 buf에 넣는다.
  sprintf(buf, "%sConnection: close\r\n", buf);
  sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
  sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
  Rio_writen(fd, buf, strlen(buf));
  printf("Response headers:\n");
  printf("%s", buf); // 응답 헤더 출력
  
  /* Send response body to client */
  srcfd = Open(filename, O_RDONLY, 0); // 함수를 호출하여 요청된 파일을 읽기 전용 모드로 연다.
  srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
  Close(srcfd); // 썼으면 제자리에
  Rio_writen(fd, srcp, filesize); // 주소 srcp에서 시작하는 filesize 크기 만큼의 바이트를 fd에 복사
  Munmap(srcp, filesize); // 메모리 누수를 막기위한 가상메모리 주소 반환
}

/*
 * get_filetype - Derive file type from filename
 * 과제 할 때 확장자 추가하기
 */
void get_filetype(char *filename, char *filetype)
{
  if (strstr(filename, ".html"))
    strcpy(filetype, "text/html");
  else if (strstr(filename, ".gif"))
    strcpy(filetype, "image/gif");
  else if (strstr(filename, ".png"))
    strcpy(filetype, "image/png");
  else if (strstr(filename, ".jpg"))
    strcpy(filetype, "image/jpeg");
  else if (strstr(filename, ".mpg"))
    strcpy(filetype, "video/mpg");
  else if (strstr(filename, ".mp4"))
    strcpy(filetype, "video/mp4");
  else
    strcpy(filetype, "text/plain");
}

// ?
void serve_dynamic(int fd, char *filename, char *cgiargs) 
{
  char buf[MAXLINE], *emptylist[] = { NULL };

  sprintf(buf, "HTTP/1.0 200 OK\r\n");
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Server: Tiny Web Server\r\n");
  Rio_writen(fd, buf, strlen(buf));

  if (Fork() == 0)
  {
    setenv("QUERY_STRING", cgiargs, 1);
    Dup2(fd, STDOUT_FILENO);
    Execve(filename, emptylist, environ); // environ: 환경변수 가리키는 리스트
  }
  Wait(NULL);
}

