#it is important to have a seperate makefile here because at somepoint we want
#to be able to just drop a new plugins source in the directory, add it to this
#make file and call make to generate the new dynamically loaded library

CC = g++

#TODO: should probably figure out how to pass this from the parent make process
INCLUDES = -I ../include \
	   -I ../include/Core \
	   -I ../../Minibot/include \
	   -I ../include/OmniComm
LIBS = '-lcurl'

PLUGINS = hurlbot.o\
	  bashbot.o

all: ${PLUGINS}

%.o: ./%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++  ${LIBS} ${INCLUDES} -g -O0 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


.PHONY:clean

clean:
	${RM} *.o *.d

