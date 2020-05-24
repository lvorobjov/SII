TESTLIB = ../src/logic.lib
INCLUDES = -I../../lab2/src -I../src
LIBS = -L../src -llogic
CXXFLAFS = -g -std=c++11 -masm=intel
DEFINES = -D_TEST_MODULE
ifndef ANSI
  DEFINES += -D_UNICODE -DUNICODE
endif
ifeq (${CC},cc)
  CC = gcc
endif

TESTBIN = $(shell pkg-config --variable=bindir cxxtest)
TESTGEN = ${TESTBIN}/cxxtestgen.pl
INCLUDES += $(shell pkg-config --cflags cxxtest)

OBJ_DIR = obj
TESTS = test1 test2 test3 test4 segfault

all: ${TESTLIB} ${TESTS} | ${OBJ_DIR}

${TESTLIB}:
	${MAKE} -C ..\src DEBUG=1 all

test1: test1.o
	${CC} -o $@ $^

memory.o: memory.c
	${CC} -std=c99 -c -o $@ $^

test1.o: test1.c
	${CC} ${DEFINES} ${INCLUDES} -std=c99 -masm=intel -c -o $@ $^

%.o: %.cpp
	${CXX} ${DEFINES} ${INCLUDES} ${CXXFLAFS} -c -o $@ $^

%.obj: %.asm
	yasm -f win32 -o $@ $<

%Test.cpp: %Test.h
	${TESTGEN} --part -o $@ $<

runner.cpp:
	${TESTGEN} --root --error-printer -o $@ $<

test2: test2.obj count.obj
	alink -oPE -o $@ $^ win32.lib -L /bin -entry _main

test3: test3.obj parser.obj string.obj
	alink -oPE -o $@ $^ win32.lib -L /bin -entry _main

test4: runner.o UtilsTest.o AttributeTest.o LogicTest.o ParserTest.o
	${CXX} -o $@ $^ ${LIBS}

segfault: segfault.o
	${CXX} -o $@ $^

memory: memory.o
	${CC} -o $@ $^

run: test4
	$<.exe

clean:
#	swiss rm -r ${OBJ_DIR}
	del *.o *.obj

${OBJ_DIR}:
	mkdir -p $@
