NAME			:= ircserv

CXX 			:= c++
CXXFLAGS  		:= -std=c++98 -Wall -Wextra -Werror -g3 -fsanitize=address 

RM				:= rm
RMFLAGS     	:= -rf

SRCDIR			:= ./srcs/

SRCNAME 		:= Client.cpp Command.cpp CommandInvoker.cpp Join.cpp Pass.cpp\
					Kick.cpp Nick.cpp Notice.cpp Part.cpp Privmsg.cpp Pong.cpp\
					Quit.cpp Server.cpp User.cpp main.cpp Channel.cpp 

SRCS			:= $(addprefix $(SRCDIR), $(SRCNAME))

OBJDIR			:= ./obj/
OBJNAME			:= $(SRCNAME:.cpp=.o)
OBJS			:= $(addprefix $(OBJDIR), $(OBJNAME))

INCDIR			:= includes/

.PHONY			: all
all				: $(NAME)

$(NAME) 		: $(OBJS)
				$(CXX) $(CXXFLAGS) -I$(INCDIR) $^ -o $@

.PHONY			: clean
clean			:
				$(RM) $(RMFLAGS) $(OBJS)
				$(RM) $(RMFLAGS) $(OBJDIR)

.PHONY			: fclean
fclean			: clean
				$(RM) $(RMFLAGS) $(NAME)

.PHONY			: re
re				: fclean all

$(OBJDIR)%.o:$(SRCDIR)%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
