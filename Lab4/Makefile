SOURCE_DIR = ./src
TESTS_DIR = ./test

all:
	make -C ${SOURCE_DIR} all
ifneq ($(wildcard ${TESTS_DIR}),)
	make -C ${TESTS_DIR} run
endif

run:
	make -C ${SOURCE_DIR} run

clean:
	make -C ${SOURCE_DIR} clean
ifneq ($(wildcard ${TESTS_DIR}),)
	make -C ${TESTS_DIR} clean
endif