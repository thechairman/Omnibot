C = g++

LIBS = -lpthread -lstdc++

INCLUDES = -I ./

OBJS = ircInterface.o\
       posix_ircio.o\
       ircNotifyClasses.o\
       main.o

all: MiniBot

-include *.d

MiniBot: ${OBJS}
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
	${RM} ${OBJS} *.d ServerController
