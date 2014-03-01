#it is important to have a seperate makefile here because at somepoint we want
#to be able to just drop a new plugins source in the directory, add it to this
#make file and call make to generate the new dynamically loaded library
DYNAMIC_LOADING = YES
CC = g++

LOAD_MODE=${}

#TODO: should probably figure out how to pass this from the parent make process
INCLUDES = -I ../include \
	   -I ../include/Core \
	   -I ../../Minibot/include \
	   -I ../include/OmniComm
LIBS = '-lcurl'

PLUGIN_OBJS = hurlbot.o\
	      bashbot.o\
	      qdbbot.o\
	      tomebot.o\
	      8ballbot.o\
	      karmabot.o\
	      wordsbot.o\
	      numberwangbot.o

PLUGINS = libhurlbot.so\
	  libbashbot.so\
	  libqdbbot.so\
	  libtomebot.so\
	  lib8ballbot.so\
	  libkarmabot.so\
	  libwordsbot.so\
	  libnumberwangbot.so

ifeq (${DYNAMIC_LOADING}, YES)
	LOAD_MODE=-DDYNAMIC_LOADING
endif

all: ${PLUGINS}

%.so: ./%.o ./OmniPluginInterface.o
	@echo 'Linking Plugin: $@'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -shared -Wl,-soname,"$@" -o "$@" "$<" OmniPluginInterface.o
	@echo 'Finished building: $<'
	@echo ' '

./OmniPluginInterface.o: ../src/Core/OmniPluginInterface.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++  ${LIBS} ${INCLUDES} -fPIC -g -O0 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib%.o: ./%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ ${LIBS} ${INCLUDES} -fPIC ${LOAD_MODE} -g -O0 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



.PHONY:clean

clean:
	${RM} *.o *.d *.so

