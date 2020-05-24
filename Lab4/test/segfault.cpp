// segfault.cpp
// Segmentation fault handling

#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <stdexcept>

void segfault(bool = false);

int main() {

	std::signal(SIGSEGV, [](int){
		throw std::runtime_error("SIGSEGV signal\n");
	});
	
	try {
		segfault();
	} catch (...) {
		printf("Exception caught.\n");
		return EXIT_FAILURE;
	}
	
	printf("End of program\n");
	return EXIT_SUCCESS;
}

void segfault(bool ill) {
	const char *str = "Test string";
	printf(ill? nullptr : "%s %s\n", str, "Other string");
}
