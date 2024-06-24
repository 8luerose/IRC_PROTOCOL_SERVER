CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11

TARGET = mode_test
SRCS = main.cpp Channel.cpp Client.cpp Error.cpp Server.cpp Util.cpp Mode.cpp Command.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
