// segfault.cpp
// Segmentation fault handling

#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <stdexcept>

struct base {
	virtual ~base() {
		safe_release();
	}
	void safe_release() {
		this->release();
	}
	virtual void release() = 0;
};

struct derived : public base {
	void release() override { };
};

template <int SIG, const char* MSG>
inline void signal_handle() {
	std::signal(SIG, [](int){
		throw std::runtime_error(MSG);
	});
}

int main() {

	static const char segv_msg[] = "Segmentation fault\n";
	static const char  ill_msg[] = "Illegal operation\n";
	static const char abrt_msg[] = "Aborted\n";

	signal_handle<SIGSEGV, segv_msg>();
	signal_handle<SIGILL,  ill_msg>();
	signal_handle<SIGABRT, abrt_msg>();
	
	try {
		derived _{};
	} catch (std::exception const& ex) {
		printf("Exception: %s\n", ex.what());
		return EXIT_FAILURE;
	} catch (...) {
		printf("Exception caught.\n");
		return EXIT_FAILURE;
	}
	
	printf("End of program\n");
	return EXIT_SUCCESS;
}
