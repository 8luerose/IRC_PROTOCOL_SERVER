SERVER=server

MAKEFLAGS += -j

RED="\033[0;31m Warining: "
GREEN="\033[3;32m"
ANCI_INIT="\033[0m"

# DEP_DIR=depend
OBJ_DIR=objs

CXX=c++

CXXFLAGS=-Wall -Wextra -Werror
CXXFLAGS+=-std=c++98
# CXXFLAGS+=-g3 -fsanitize=address
# CXXFLAGS+=-MMD -MP -MF $(DEP_DIR)/$*.d

RM=rm -rf

SRCS_SERVER=main.cpp \
			src/Channel.cpp \
			src/Client.cpp \
			src/Command.cpp \
			src/Error.cpp \
			src/Msg.cpp \
			src/Server.cpp \
			src/Util.cpp \
			command/Join.cpp \
			command/Nick.cpp \
			command/User.cpp \

# Replace .cpp with .o and add OBJ_DIR prefix
# OBJS_SERVER=$(addprefix $(OBJ_DIR)/, $(SRCS_SERVER:.cpp=.o))
OBJS_SERVER = $(SRCS_SERVER:.c=.o)

all: $(SERVER)

$(SERVER): $(OBJS_SERVER)
	@$(CXX) $(CXXFLAGS) $(OBJS_SERVER) $(LIBFT) -o $(SERVER)

$(OBJ_DIR)/%.o: %.cpp
# @mkdir -p $(dir $@) # Create the necessary directories
# @mkdir -p $(DEP_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@ \
	&& echo $(GREEN)"Compiled successful!" $<$(ANCI_INIT) \
	|| echo $(RED)"Failed to compile $<"$(ANCI_INIT);

clean:
# @$(RM) $(OBJS_SERVER); $(RM); $(RM) $(OBJ_DIR) $(DEPD) $(DEP_DIR)
	@$(RM) $(OBJS_SERVER); $(RM) $(OBJ_DIR)\
	&& echo $(GREEN)"Clean successful!"$(ANCI_INIT) \
	|| echo $(RED)"Clean failed!"$(ANCI_INIT);

fclean: clean
	@$(RM) $(SERVER); $(RM) \
	&& echo $(GREEN)"fclean successful!"$(ANCI_INIT) \
	|| echo $(RED)"fclean failed!"$(ANCI_INIT);

re: fclean all

.PHONY: all clean fclean re
