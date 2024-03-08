cmake_minimum_required(VERSION 3.26)
project(ircserv)

set(CMAKE_CXX_STANDARD 14)

include_directories(./srcs/, ./srcs/commands)

add_executable(
        ircserv
        srcs/Channel.cpp
        srcs/Channel.hpp
        srcs/ChannelMenager.cpp
        srcs/ChannelMenager.hpp
        srcs/Client.cpp
        srcs/Client.hpp
        srcs/Colors.h
        srcs/Get.cpp
        srcs/Get.hpp
        srcs/Irc.hpp
        srcs/main.cpp
        srcs/RPL.cpp
        srcs/RPL.hpp
        srcs/Server.cpp
        srcs/Server.hpp
        srcs/commands/ACommand.cpp
        srcs/commands/ACommand.hpp
        srcs/commands/CAP.cpp
        srcs/commands/CAP.hpp
        srcs/commands/INVITE.cpp
        srcs/commands/INVITE.hpp
        srcs/commands/JOIN.cpp
        srcs/commands/JOIN.hpp
        srcs/commands/KICK.cpp
        srcs/commands/KICK.hpp
        srcs/commands/MODE.cpp
        srcs/commands/MODE.hpp
        srcs/commands/NICK.cpp
        srcs/commands/NICK.hpp
        srcs/commands/PART.cpp
        srcs/commands/PART.hpp
        srcs/commands/PASS.cpp
        srcs/commands/PASS.hpp
        srcs/commands/PING.cpp
        srcs/commands/PING.hpp
        srcs/commands/PRIVMSG.cpp
        srcs/commands/PRIVMSG.hpp
        srcs/commands/QUIT.cpp
        srcs/commands/QUIT.hpp
        srcs/commands/SQUIT.cpp
        srcs/commands/SQUIT.hpp
        srcs/commands/TOPIC.cpp
        srcs/commands/TOPIC.hpp
        srcs/commands/USER.cpp
        srcs/commands/USER.hpp
        srcs/commands/WHO.cpp
        srcs/commands/WHO.hpp
        srcs/commands/WHOIS.cpp
        srcs/commands/WHOIS.hpp
        srcs/commands/QUIT.cpp

)