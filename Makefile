CPPF = c++ -Wall -Werror -Wextra -std=c++98 -g

NAME = ircserv

SRCS = 	srcs/main.cpp  srcs/Server.cpp srcs/Client.cpp \
		srcs/Get.cpp srcs/RPL.cpp srcs/Channel.cpp srcs/ChannelMenager.cpp\
		srcs/commands/ACommand.cpp \
		srcs/commands/PASS.cpp \
		srcs/commands/NICK.cpp \
		srcs/commands/USER.cpp \
		srcs/commands/PING.cpp \
		srcs/commands/PART.cpp \
		srcs/commands/JOIN.cpp \
		srcs/commands/PRIVMSG.cpp \
		srcs/commands/KICK.cpp \
		srcs/commands/WHO.cpp \
		srcs/commands/MODE.cpp \
		srcs/commands/TOPIC.cpp \
		srcs/commands/WHOIS.cpp \
		srcs/commands/INVITE.cpp \
		srcs/commands/SQUIT.cpp \
		srcs/commands/CAP.cpp \
		srcs/commands/QUIT.cpp \

# Object files 
OBJS = ${SRCS:.cpp=.o}

RM = rm -f

BLUE = \033[1;34m
END	 = \033[0m

all: ${NAME}

%.o: %.cpp
	${CPPF} -c $< -o $@

${NAME} : ${OBJS}
	${CPPF} $^ -o $@
	@echo "${BLUE}To start this program, type ./${NAME} <port> <password>${END}"

# Rule to clean the compiled files
clean:
	${RM} ${OBJS} ${NAME}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re