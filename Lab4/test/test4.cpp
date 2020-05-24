// test4.cpp
// Testing parser of logic.lib

#include <stdio.h>
#include "parser.h"

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

#include <stdlib.h>
#include <signal.h>

#include <windows.h>

bool testUtils();
bool testLogic();
bool testParser();

bool testParseAttribute();
bool testParseStatement();
bool testParseRule();

void segv_signal_handle(int signum) {
	if (signum == SIGSEGV) {
		TS_FAIL("Segmentation fault");
	} else if (signum == SIGILL) {
		TS_FAIL("Illegal instruction");
	} else if (signum == SIGFPE) {
		TS_FAIL("Floating point exception");
	} else if (signum == SIGINT) {
		TS_FAIL("Interruption");
	}
	signal(signum, SIG_DFL);
	exit(3);
}


LONG WINAPI UEF(
  _EXCEPTION_POINTERS *ExceptionInfo
) {
	TS_FAIL("Unhandle exception");
	return EXCEPTION_EXECUTE_HANDLER;
}


int main() {
	signal(SIGINT, segv_signal_handle);
	signal(SIGSEGV, segv_signal_handle);
	signal(SIGFPE, segv_signal_handle);
	signal(SIGILL, segv_signal_handle);
	SetUnhandledExceptionFilter (UEF);
#if 0
	__asm__ __volatile__ (
		".intel_syntax noprefix\n\t"
		"push %0\n\t"
		"push qword ptr [fs:0]\n\t"
		"mov  [fs:0],rsp\n\t"
		:
		: "r" (UEF)
	);
#endif
 	return CxxTest::ErrorPrinter().run();
}
#include <cxxtest/Root.cpp>
