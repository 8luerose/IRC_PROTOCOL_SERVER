SERVER=server

MAKEFLAGS += -j

RED="\033[0;31m Warining: "
GREEN="\033[3;32m"
ANCI_INIT="\033[0m"

CXX=c++

CXXFLAGS=-Wall -Wextra -Werror
CXXFLAGS+=-std=c++98
# CXXFLAGS+=-g3 -fsanitize=address

RM=rm -rf

SRCS_SERVER= main.cpp \
			src/Channel.cpp \
			src/Client.cpp \
			src/Command.cpp \
			src/Error.cpp \
			src/Msg.cpp \
			src/Server.cpp \
			src/Util.cpp \
			Command/Invite.cpp \
			Command/Join.cpp \
			Command/Kick.cpp \
			Command/Mode.cpp \
			Command/Nick.cpp \
			Command/Part.cpp \
			Command/Pass.cpp \
			Command/Privmsg.cpp \
			Command/Quit.cpp \
			Command/Topic.cpp \
			Command/User.cpp


OBJS_SERVER = $(SRCS_SERVER:.cpp=.o)

all: $(SERVER)

$(SERVER): $(OBJS_SERVER)
	@$(CXX) $(CXXFLAGS) $(OBJS_SERVER) $(LIBFT) -o $(SERVER) \

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@ \
	&& echo $(GREEN)"Compiled successful!" $<$(ANCI_INIT) \
	|| echo $(RED)"Failed to compile $<"$(ANCI_INIT);

clean:
	@$(RM) $(OBJS_SERVER) \
	&& echo $(GREEN)"Clean successful!"$(ANCI_INIT) \
	|| echo $(RED)"Clean failed!"$(ANCI_INIT);

fclean: clean
	@$(RM) $(SERVER) \
	&& echo $(GREEN)"fclean successful!"$(ANCI_INIT) \
	|| echo $(RED)"fclean failed!"$(ANCI_INIT);

re: fclean all

.PHONY: all clean fclean re
