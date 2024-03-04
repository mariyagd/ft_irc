NAME = ircserv
SRCS = 	main.cpp  Server.cpp Client.cpp \
		Get.cpp RPL.cpp Channel.cpp ChannelMenager.cpp\
		ACommand.cpp \
		PASS.cpp \
		NICK.cpp \
		USER.cpp \
		PING.cpp \
		PART.cpp \
		JOIN.cpp \
		PRIVMSG.cpp \
		KICK.cpp \
		WHO.cpp \
		MODE.cpp \
		TOPIC.cpp \
		WHOIS.cpp \
		INVITE.cpp \

CPPF = c++ -Wall -Werror -Wextra -std=c++98 -g

RM = rm -f

BLUE = \033[1;34m
END	 = \033[0m

all: ${NAME}

${NAME} :
	${CPPF} ${SRCS} -o ${NAME}
	@echo "${BLUE}To start this program, type ./${NAME} <port> <password>${END}"

fclean:
	${RM} ${NAME}

re: fclean all

.PHONY: re fclen all
