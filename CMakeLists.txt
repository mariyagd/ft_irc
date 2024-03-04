cmake_minimum_required(VERSION 3.26)
project(ft_irc)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(
        ft_irc
        Irc.hpp
        main.cpp
        Server.cpp
        Server.hpp
        Client.cpp
        Colors.h
        RPL.cpp
        RPL.hpp
        Commands.hpp
        Commands.cpp
        Channel.cpp
        ChannelMenager.cpp
        Get.cpp
        ACommand.hpp
        ACommand.hpp
        ACommand.hpp
        ACommand.cpp
        PASS.hpp
        PASS.cpp
        NICK.cpp
        NICK.cpp
        USER.hpp
        USER.cpp
        JOIN.cpp
        INVITE.cpp
        KICK.cpp
        MODE.cpp
        PART.cpp
        PART.cpp
        PING.cpp
        PRIVMSG.cpp
        WHO.cpp
        WHOIS.cpp
        TOPIC.cpp
)
