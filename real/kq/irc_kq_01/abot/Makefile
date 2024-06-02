NAME			:= abot

CXX 			:= c++
CXXFLAGS  		:= -std=c++98 -Wall -Wextra -Werror

RM				:= rm
RMFLAGS     	:= -rf

SRCS			:= main.cpp Bot.cpp

OBJS			:= $(SRCS:.cpp=.o)

.PHONY			: all
all				: $(NAME)

$(NAME) 		: $(OBJS)
				$(CXX) $^ -o $@

.PHONY			: clean
clean			:
				$(RM) $(RMFLAGS) $(OBJS)

.PHONY			: fclean
fclean			: clean
				$(RM) $(NAME)

.PHONY			: re
re				: fclean all

.cpp.o:
	$(CXX) -c $< -o $@