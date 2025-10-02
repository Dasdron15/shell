BIN_NAME=shell

all:
	mkdir -p bin
	${CC} -o bin/${BIN_NAME} *.c
