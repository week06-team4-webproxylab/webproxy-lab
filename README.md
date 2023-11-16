####################################################################
# CS:APP Proxy Lab
#
# Student Source Files
####################################################################

This directory contains the files you will need for the CS:APP Proxy
Lab.

proxy.c
csapp.h
csapp.c
    These are starter files.  csapp.c and csapp.h are described in
    your textbook. 

    You may make any changes you like to these files.  And you may
    create and handin any additional files you like.

    Please use `port-for-user.pl' or 'free-port.sh' to generate
    unique ports for your proxy or tiny server. 

Makefile
    This is the makefile that builds the proxy program.  Type "make"
    to build your solution, or "make clean" followed by "make" for a
    fresh build. 

    Type "make handin" to create the tarfile that you will be handing
    in. You can modify it any way you like. Your instructor will use your
    Makefile to build your proxy from source.

port-for-user.pl
    Generates a random port for a particular user
    usage: ./port-for-user.pl <userID>

free-port.sh
    Handy script that identifies an unused TCP port that you can use
    for your proxy or tiny. 
    usage: ./free-port.sh

driver.sh
    The autograder for Basic, Concurrency, and Cache.        
    usage: ./driver.sh

nop-server.py
     helper for the autograder.         

tiny
    Tiny Web server from the CS:APP text

####################################################################
# CS:APP 프록시 랩
#
# 학생 소스 파일
####################################################################
이 디렉토리에는 CS:APP 프록시 랩을 위해 필요한 파일들이 있습니다.
proxy.c
csapp.h
csapp.c
    이들은 시작 파일들입니다. csapp.c와 csapp.h는 교재에서 설명되어 있습니다.
  
    이 파일들에 원하는 변경을 할 수 있습니다. 그리고 원하는 추가 파일을 생성하고 제출할 수도 있습니다.
  
    프록시 또는 Tiny 서버에 대해 고유한 포트를 생성하기 위해 'port-for-user.pl' 또는 'free-port.sh'를 사용해 주세요.

Makefile
    프록시 프로그램을 빌드하는 메이크파일입니다. 솔루션을 빌드하려면 "make"를 입력하거나, 새로 빌드하기 위해 "make clean" 다음에 "make"를 입력하세요.
  
    제출할 tar 파일을 생성하기 위해 "make handin"을 입력하세요. 원하는 대로 수정할 수 있습니다. 교수님은 제출한 Makefile을 사용하여 소스에서 프록시를 빌드할 것입니다.

port-for-user.pl
    특정 사용자를 위해 무작위 포트를 생성합니다.
    사용법: ./port-for-user.pl <userID>

free-port.sh
    프록시나 Tiny를 위해 사용되지 않는 TCP 포트를 식별하는 유용한 스크립트입니다.
    사용법: ./free-port.sh

driver.sh
    기본, 동시성, 캐시에 대한 자동채점 프로그램.       
    사용법: ./driver.sh

nop-server.py
    자동채점을 위한 도우미.        

tiny
    CS:APP 교재에서 나오는 Tiny 웹 서버