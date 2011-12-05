VPATH = ../Minibot
CC = g++

LIBS = -lpthread -lstdc++

INCLUDES = -I ./ -I ../Minibot/

OBJS = ircInterface.o\
       posix_ircio.o\
       ircNotifyClasses.o\
       omnibot.o\
       PluginFactory.o\
       PluginManager.o\
       LinuxPluginFactory.o\
       PluginUtils.o\
       OmniPluginInterface.o\
       OmniThread.o\
       main.o


all: Omnibot

-include *.d

Omnibot: ${OBJS}
	@echo "Building: $@"
	${CC} -o"$@" ${LIBS} ${OBJS}
	@echo "Done building: $@"

%.o : ./%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ ${INCLUDES} -O0 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



clean:
	${RM} ${OBJS} *.d Omnibot
