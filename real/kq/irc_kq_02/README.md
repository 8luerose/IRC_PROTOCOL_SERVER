# ft_irc 💬
Simple IRC Server following the IRC protocol specifications. Some concepts we learned were: how to create a server with C++ and handle connections in a non-blocking way without using threads.
Developed by @gpernas- and @karisti-.

[![Image from Gyazo](https://i.gyazo.com/175f5e05408824fa0f46a7feccb27697.gif)](https://gyazo.com/175f5e05408824fa0f46a7feccb27697)

## References RFC-IRC PROTOCOL
- https://www.ietf.org/rfc/rfc1459.txt
- https://www.rfc-es.org/rfc/rfc1459-es.txt
- https://modern.ircdocs.horse/#client-to-server

## IRC Clients
These are some clients we used and are easy to set
- [CIRC](https://chrome.google.com/webstore/detail/circ/bebigdkelppomhhjaaianniiifjbgocn) - Only works on Chrome! 
- [kiwiIRC](https://kiwiirc.com/)

## Resources learned
- [TCP/IP Protocol in C++](https://lenngro.github.io/how-to/2021/01/05/Simple-TCPIP-Server-Cpp/)
- [In-Out events (Poll/Select/Kqueue)](https://nima101.github.io/io_multiplexing)
- [Kqueue](https://dev.to/frevib/a-tcp-server-with-kqueue-527)
- [Non-Blocking fd C++](https://www.linuxtoday.com/blog/blocking-and-non-blocking-i-0/)
- [Sockets](https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf) (Pag 60)

  ## Usage
- Run `$make && ./ircserv <SERVER_PORT> <SERVER_PASSWORD>`
- Connect from client. From CIRC: /server <SERVER_IP> <SERVER_PORT> <SERVER_PASSWORD>

<!--
## CONCEPTS
* BSD Sockets
* Receive commands via recv()
* Check Alive status with PINGS that must be responded with /PONG time()
* Concurrency run() livelock
* Multithreading -> main
                    send() [OPTIONAL]
                    receive()
* Serializacion
-->
# Project Examples
- [TPC Server Linux](https://www.youtube.com/watch?v=cNdlrbZSkyQ)
- [TPC Server Mac](https://www.youtube.com/watch?v=F3iIGUiW27Q)

<!--
/*

pass 12
nick karisti1
user karisti2 0 * :Kepa
join #aaaa,#bbbb


pass 12
nick gpernas1
user gpernas2 0 * :Gonzalo
join #aaaa,#bbbb

pass 12
nick gpernas
user gpernas1 0 * :Gonzalo
join #test

*/


https://modern.ircdocs.horse/#connection-setup

nc irc.irc-hispano.org 6667
lsof -t -i tcp:6667 | xargs kill



https://github.com/abambi-abambi/irc_server/blob/master/additional_materials/checklist_ft_irc_en.pdf

join #1,#2,#3,#4,#5,#6,#7,#8,#9,#10,#11,#12,#13

-->
