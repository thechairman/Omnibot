VPATH = ./src

CC = g++
AR = ar
LIBS = -lpthread -lstdc++
INCLUDES = -I ./include

BUILD_DIR = ./build/

LIBRARY = ${BUILD_DIR}libMinibot.a
EXECUTABLE = ${BUILD_DIR}Minibot

#All objects not specifically related to executables go here
LIB_OBJS = ${BUILD_DIR}ircInterface.o\
	   ${BUILD_DIR}posix_ircio.o\
	   ${BUILD_DIR}posix_ircConnStatus.o\
	   ${BUILD_DIR}ircConnStatus.o\
	   ${BUILD_DIR}ircNotifyClasses.o\
	   ${BUILD_DIR}ircio.o\
	   ${BUILD_DIR}ircNickListParser.o

#all objects related only to the executable
EXE_OBJS = ${LIB_OBJS}\
	   ${BUILD_DIR}main.o
#list of all products to build
PRODUCTS = ${EXECUTABLE}\
	   ${LIBRARY}

all: ${PRODUCTS}

-include ${BUILD_DIR}*.d

${LIBRARY}: ${LIB_OBJS}
	@mkdir -p ${BUILD_DIR}
	@echo "Building: $@"
	${AR} -rv $@ ${LIB_OBJS}	
	@echo "Done building: $@"
	@echo ' '

${EXECUTABLE}: ${EXE_OBJS}
	@mkdir -p ${BUILD_DIR}
	@echo "Building: $@"
	${CC} -o"$@" ${LIBS} ${EXE_OBJS}
	@echo "Done building: $@"
	@echo ' '

${BUILD_DIR}%.o : ./src/%.cpp
	@mkdir -p ${BUILD_DIR}
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ ${INCLUDES} -D_GLIBCXX_DEBUG -g -O0 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

.PHONY: clean
clean:
	${RM} ${BUILD_DIR}*.o ${BUILD_DIR}*.d ${PRODUCTS}
