# Docker를 사용한 IRC 서버 구축 가이드

이 문서는 Docker를 사용하여 간단한 IRC 서버를 빌드하고 실행하는 방법을 안내합니다.

## 필수 조건
- Docker 설치
- 터미널

## 시작하기
### Makefile 사용
1. `make all` 명령어를 실행하여 컨테이너를 빌드하고 실행합니다.
2. 컨테이너가 실행되면 다음과 같은 출력 문구가 나타납니다.


```
✔ Network irc-project_default	Created                                        0.0s
✔ Container irc       			Started
```

3. 3개의 터미널 탭을 엽니다.

### IRC 클라이언트 연결
1. 1번 탭: `docker logs -f irc`를 입력하여 서버 로그를 확인합니다.
2. 2번 탭:
   - `docker exec -it irc /bin/bash`를 입력하여 root 계정으로 접속합니다.
   - `irssi` 명령어를 사용하여 irssi에 접속합니다.
   - '/connect {hostip} 4000 1234' 명령어를 사용하여 서버에 연결합니다.
		- ifconfig -> en0 -> inet -> {ex) hostip == 192.168.0.71} 으로 확인가능
   - `/NICK player1`을 입력하여 닉네임을 설정합니다.
   - `/JOIN #general`을 입력하여 채널에 입장합니다.
3. 3번 탭:
   - (2번 탭과 동일)
   - `docker exec -it irc /bin/bash`를 입력하여 root 계정으로 접속합니다.
   - `irssi` 명령어를 사용하여 irssi에 접속합니다.
   - '/connect {hostip} 4000 1234' 명령어를 사용하여 서버에 연결합니다.
    	- ifconfig -> en0 -> inet -> {ex) hostip == 192.168.0.71} 으로 확인가능
   - `/NICK player2`를 입력하여 닉네임을 설정합니다.
   - `/JOIN #general`을 입력하여 채널에 입장합니다.

### 채팅
  - 2번 탭과 3번 탭에서 채팅 메시지를 입력하여 서로 대화할 수 있습니다.
  - ex) "hello" 입력 시, 다른 사용자에게 "player1: hello" 또는 "player2: hello" 메시지가 전송됩니다.
  - nc에서는 PRIVMSG #general :hello로 입력하는 것과 같습니다.

## Makefile 사용법
  - `make all`: 컨테이너를 빌드하고 실행합니다.
  - `make build`: 컨테이너 이미지를 빌드합니다.
  - `make up`: 컨테이너를 실행합니다.
  - `make down`: 컨테이너를 중지하고 제거합니다.
  - `make start`: 중지된 컨테이너를 다시 시작합니다.
  - `make stop`: 실행 중인 컨테이너를 중지합니다.
  - `make clean`: 컨테이너를 중지하고 이미지 및 네트워크를 제거합니다.
  - `make fclean`: 컨테이너, 볼륨, 네트워크 및 이미지를 모두 제거합니다.
  - `make ffclean`: 컨테이너, 볼륨, 네트워크, 이미지 및 시스템 prune를 모두 제거합니다.
  - `make re`: 모든 것을 제거하고 다시 빌드합니다.

## 사용 가능한 IRC 명령어
  - `PASS`: 서버에 연결하기 위한 패스워드를 확인합니다.
  - `NICK`: 닉네임을 변경합니다.
  - `USER`: 유저 이름을 변경합니다.
  - `JOIN`: 채널에 입장합니다.
  - `PART`: 채널에서 나갑니다.
  - `PRIVMSG`: 특정 사용자 또는 채널에 메시지를 보냅니다.
  - `NOTICE`: 서버의 유저에게 메시지를 보냅니다.
  - `LIST`: 현재 서버에서 사용 가능한 채널 목록을 조회합니다.
  - `PING`: 클라이언트-서버 간의 연결을 확인합니다.
  - `OPER`: 관리자 권한을 얻습니다.
  - `KICK`: 유저를 특정 채널에서 내보냅니다.
  - `QUIT`: IRC 서버에서 나갑니다.

## IRC 명령어 예제
1. 닉네임 변경: `/NICK 새로운_닉네임` (예시: `/NICK 홍길동`)
2. 채널 입장: `/JOIN 채널명` (예시: `/JOIN #general`)
3. 채널 나가기: `/PART 채널명` (예시: `/PART #general`)
4. 특정 사용자에게 메시지 보내기: `/PRIVMSG 사용자명 메시지 내용` (예시: `/PRIVMSG taehkwon 안녕하세요!`)
5. 서버에 메시지 보내기: `/NOTICE 서버 메시지 내용` (예시: `/NOTICE 서버 테스트 메시지입니다.`)
6. 사용 가능한 채널 목록 확인: `/LIST`
7. 서버 연결 확인: `/PING`
8. 관리자 권한 얻기: `/OPER`
9. 특정 사용자를 채널에서 내보내기: `/KICK 채널명 사용자명` (예시: `/KICK #general 홍길동`)
10. IRC 서버 종료: `/QUIT`

