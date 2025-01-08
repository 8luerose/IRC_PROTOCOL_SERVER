# I/O Multiplexing과 IRC 서버 구현

## 📌 Overview
IRC(Internet Relay Chat)는 실시간 채팅을 위한 프로토콜로, 다수의 클라이언트가 동시에 접속하여 통신하는 시스템입니다. I/O Multiplexing은 이러한 IRC 서버 구현에 필수적인 기술로, 단일 프로세스에서 다수의 클라이언트 연결을 효율적으로 관리합니다.



## I/O Multiplexing의 본질적 이해

I/O Multiplexing은 단일 프로세스로 여러 입출력을 동시에 처리하는 기술입니다. 이를 쉽게 이해하기 위해 도서관 사서의 업무를 예로 들어보겠습니다. 전통적인 방식의 사서(단일 프로세스)는 한 번에 한 명의 방문자(I/O 요청)만 응대할 수 있어, 다른 방문자들은 줄을 서서 기다려야 합니다. 반면 I/O Multiplexing을 적용한 사서는 여러 방문자의 요청을 동시에 받아들이고, 책을 찾는 동안 다른 방문자의 요청도 처리할 수 있습니다. 이것이 바로 하나의 프로세스로 여러 I/O를 효율적으로 관리하는 I/O Multiplexing의 핵심입니다.

## Kqueue를 활용한 비동기 + 커널 Blocking 구현

BSD 계열 시스템에서 사용되는 Kqueue는 이러한 I/O Multiplexing을 구현하는 고성능 방식입니다. Kqueue의 동작을 커피숍 주문 시스템에 비유해보겠습니다. 바리스타(커널)는 주문(I/O 요청)을 받으면 즉시 주문번호(이벤트)를 고객(프로세스)에게 제공합니다. 고객은 주문번호를 들고 있다가 음료가 준비되었다는 호출(이벤트 발생)을 기다립니다. 이때 고객은 카페에서 기다리면서 다른 활동(다른 작업)을 할 수 있습니다. 이것이 바로 비동기 + 커널 Blocking의 개념입니다.

kqueue는 마치 카페의 주문 관리 시스템처럼 작동합니다. 새로운 고객(클라이언트)이 들어오면 주문(연결)을 받고, 각 고객의 요청(메시지)을 비동기적으로 처리합니다. kevent 함수는 바리스타가 주문을 처리하는 것처럼 클라이언트의 요청을 처리하며, 이 과정에서 서버는 다른 연결도 동시에 관리할 수 있습니다. 이러한 방식으로 단일 프로세스로도 수천 개의 동시 연결을 효율적으로 처리할 수 있게 됩니다.



## 🌟 IRC와 I/O Multiplexing의 관계

### IRC 서버의 특징
- 다수의 클라이언트 동시 연결 처리
- 실시간 메시지 브로드캐스팅
- 채널 관리 및 사용자 상태 관리
- 낮은 지연시간 요구

### I/O Multiplexing의 필요성
- **다중 연결 처리**: 수천 개의 클라이언트 연결을 단일 프로세스에서 관리
- **실시간 성능**: 메시지 전달의 지연 최소화
- **리소스 효율성**: 클라이언트당 스레드 생성 없이 효율적인 자원 관리



## 사용방법 (How to Use)
1. IRC_PROTOCOL_SERVER 폴더로 이동합니다.
2. `make` 명령어를 실행하여 프로젝트를 빌드합니다.
3. docker_ALL 폴더로 이동합니다. (/docker_ALL/README.md 파일 참고)
4. `make` 명령어를 실행하여 컨테이너를 빌드하고 실행합니다.
5. 컨테이너가 실행되면 다음과 같은 출력 문구가 나타납니다.


```
✔ Network irc-project_default	Created                                        0.0s
✔ Container irc       			Started
```

6. 3개의 터미널 탭을 엽니다.

### (1) FT_IRC와 - 클라이언트 연결
1. 1번 탭: 로컬 맥북에서 `./ircserv 4000 1234`를 입력하여 서버 로그를 확인합니다.
2. 2번 탭:
   - `docker exec -it irc /bin/bash`를 입력하여 root 계정으로 접속합니다.
   - `irssi` 명령어를 사용하여 irssi에 접속합니다.
   - `/connect {hostip} 4000 1234` 명령어를 사용하여 서버에 연결합니다.
		- ifconfig -> en0 -> inet -> {ex) hostip == 192.168.0.71} 으로 확인가능
   - `/NICK player1`을 입력하여 닉네임을 설정합니다.
   - `/JOIN #general`을 입력하여 채널에 입장합니다.
3. 3번 탭:
   - (2번 탭과 동일)
   - `docker exec -it irc /bin/bash`를 입력하여 root 계정으로 접속합니다.
   - `nc {hostip} 4000` 명령어를 사용하여 nc에 접속합니다.
   - `PASS 1234`를 입력하여 서버에 연결합니다.
   - `NICK player2`를 입력하여 닉네임을 설정합니다.
   - `USER player2 0 * :player2`를 입력하여 유저 이름을 설정합니다.
   - `JOIN #general`를 입력하여 채널에 입장합니다.





## 💡 Technical Implementation

### 1. I/O Multiplexing 구현 방식

#### 기존 Select 기반 구현
```c
fd_set readfds;
FD_ZERO(&readfds);
FD_SET(server_socket, &readfds);
select(max_fd + 1, &readfds, NULL, NULL, NULL);
```

#### Kqueue (BSD 계열)
```c
struct kevent {
    uintptr_t ident;      // 이벤트 식별자
    int16_t   filter;     // 이벤트 필터
    uint16_t  flags;      // 처리 플래그
    uint32_t  fflags;     // 필터 별 플래그
    intptr_t  data;       // 필터 별 데이터
    void      *udata;     // 사용자 정의 데이터
    uint64_t  ext[4];     // 확장 데이터
};
```

#### Kqueue의 주요 특징
1. **커널 이벤트 큐 관리**
   - kqueue(): 커널에 이벤트 큐 생성 요청
   - kevent(): 큐의 이벤트 등록/감시

2. **이벤트 필터링**
   - EVFILT_READ: 읽기 가능 이벤트
   - EVFILT_WRITE: 쓰기 가능 이벤트
   - EVFILT_EMPTY: 버퍼 비움 이벤트
   - EVFILT_VNODE: 파일 변경 이벤트
   - EVFILT_PROC: 프로세스 상태 변경
   - EVFILT_SIGNAL: 시그널 발생
   - EVFILT_TIMER: 타이머 이벤트

3. **이벤트 플래그**
   ```c
   EV_ADD      // 이벤트 추가
   EV_ENABLE   // 이벤트 활성화
   EV_DISABLE  // 이벤트 비활성화
   EV_DELETE   // 이벤트 제거
   EV_ONESHOT  // 1회성 이벤트
   EV_CLEAR    // 이벤트 상태 초기화
   ```



### 2. IRC 프로토콜 구현

#### 메시지 처리
```plaintext
:<prefix> <command> <params> :<trailing>
예시: :nick!user@host PRIVMSG #channel :Hello, World!
```

## 사용 가능한 IRC 명령어
  - `PASS`: 서버에 연결하기 위한 패스워드를 확인합니다.
  - `NICK`: 닉네임을 변경합니다.
  - `USER`: 유저 이름을 변경합니다.
  - `JOIN`: 채널에 입장합니다.
  - `PART`: 채널에서 나갑니다.
  - `PRIVMSG`: 특정 사용자 또는 채널에 메시지를 보냅니다.(IRSSI에서는 /msg 사용)
  - `NOTICE`: 서버의 유저에게 메시지를 보냅니다.
  - `LIST`: 현재 서버에서 사용 가능한 채널 목록을 조회합니다.
  - `PING`: 클라이언트-서버 간의 연결을 확인합니다.
  - `MODE`: 채널의 모드를 변경합니다.
  - `KICK`: 유저를 특정 채널에서 내보냅니다.
  - `QUIT`: IRC 서버에서 나갑니다.

## IRC 명령어 예제
1. 닉네임 변경: `/NICK 새로운_닉네임` (예시: `/NICK 홍길동`)
2. 채널 입장: `/JOIN 채널명` (예시: `/JOIN #general`)
3. 채널 나가기: `/PART 채널명` (예시: `/PART #general`)

4. 개인 사용자와 1:1 채팅: `/MSG 사용자명 메시지` (예시: `/PRIVMSG taehkwon 안녕하세요!`)
	- nc: `PRIVMSG taehkwon :안녕하세요!`
5. 채널에 메시지 보내기: `/MSG #채널명 메시지` (예시: `/PRIVMSG #general 안녕하세요!`)
	- nc: `PRIVMSG #general :안녕하세요!`
6. 채널 사용자와 대화: `/MSG #채널명 사용자명 메시지` (예시: `/PRIVMSG #general taehkwon 안녕하세요!`)
	- nc: `PRIVMSG #general taehkwon :안녕하세요!`

7. 채널 모드 사용법: `/MODE 채널명 +iktlo` (+는 추가, -는 제거)
	- `+i`: 초대 전용 채널로 설정합니다.
	- `+k`: 비밀번호를 설정합니다. (예시: `/MODE #general +k password`)
	- `+t`: 주제 변경을 채널 관리자만 할 수 있도록 설정합니다. (예시: `/MODE #general +t apple`)
	- `+l`: 채널에 입장할 수 있는 사용자 수를 제한합니다. (예시: `/MODE #general +l 5`)
	- `+o`: 사용자에게 채널 관리자 권한을 부여합니다. (예시: `/MODE #general +o taehkwon`)

8. 서버에 메시지 보내기: `/NOTICE 서버 메시지 내용` (예시: `/NOTICE 서버 테스트 메시지입니다.`)
9. 사용 가능한 채널 목록 확인: `/LIST`
10. 서버 연결 확인: `/PING`
11. 특정 사용자를 채널에서 내보내기: `/KICK 채널명 사용자명` (예시: `/KICK #general 홍길동`)
12. IRC 서버 종료: `/QUIT`



## 📝 참고 사항
- IRC RFC 문서: RFC 1459, 2812
- BSD kqueue 매뉴얼
- 네이버클라우드 기술 블로그

