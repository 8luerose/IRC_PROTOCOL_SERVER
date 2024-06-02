#include "Bot.hpp"

// test server
// /connect irc.atw-inter.net 6667
// nc -c 94.125.182.253 6667

int main(int ac, char*av[]){
  if (ac == 3){
    Bot *bot1 = new Bot(av[1], atoi(av[2]));
    bot1->wingChicken();
    delete bot1;
  }
  else if (ac == 4){
    Bot *bot1 = new Bot(av[1], atoi(av[2]), av[3]);
    bot1->wingChicken();
    delete bot1;
  }
  else{
    std::cerr << "check argument\n";
    return 1;
  }
  return 0;
}