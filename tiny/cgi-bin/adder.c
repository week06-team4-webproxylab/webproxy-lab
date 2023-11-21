/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
/* $begin adder */
#include "csapp.h"

int main(void) 

{
  char *buf, *p;
  char arg1[MAXLINE], arg2[MAXLINE], content[MAXLINE];
  int n1 = 0, n2 = 0;
            // CSAPP 8.4에 설명 있음, 쿼리 스트링 반환, buf는 포인터
  if ((buf = getenv("QUERY_STRING")) != NULL)
  { // buf에 쿼리 스트링 들어가있다.
    printf("adder.c의 buf: %s\n", buf);
    
    p = strchr(buf, '&');
    *p = '\0'; // `&`를 NULL로 바꿈
    // strcpy(arg1, buf);
    // strcpy(arg2, p+1);
    // n1 = atoi(arg1);
    // n2 = atoi(arg2);

    sscanf(buf, "first=%d", &n1);
    sscanf(p+1, "second=%d", &n2);
  }

  /* Make the response body */ 
  sprintf(content, "QUERY_STRING=%s\r\n", buf); // buf를 %s에 대입하고, 전체 문자열을 content에 넣는다.
  sprintf(content, "Welcome to add.com: "); // 하지만 여기에서 content가 덮어 씌워짐 
                                            // 따라서 위에 buf는 content에 buf를 전달하기 위한 
                                            // 매개로 쓰였다는 것을 유추 할 수 있다.
  sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
  sprintf(content, "%sThe answer is: %d + %d = %d\r\n<p>",
          content, n1, n2, n1 + n2);
  sprintf(content, "%sThanks for visiting!\r\n", content);

  /* Generate the HTTP response */
  printf("Connection: close\r\n");
  printf("Content-length: %d\r\n", (int)strlen(content));
  printf("Content-type: text/html\r\n\r\n");
  printf("%s", content);
  fflush(stdout);

  exit(0);
} 

  
/* $end adder */
