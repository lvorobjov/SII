#include <cstdlib>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#include <io.h> // setmode
#include <fcntl.h>
#endif

#include "parser.h"
#include "table.h"
#include "filemap.h"

using namespace sii;

int main(int argc, char *argv[]) {
#ifdef _WIN32
    char *__progname = argv[0];
#else
    extern char *__progname;
#endif
#ifdef _VERBOSE
	if (argc < 3) {
        printf("Usage: %s input.txt output.tex", __progname);
		return EXIT_FAILURE;
	}
#else
	if (argc < 2) {
        printf("Usage: %s input.txt", __progname);
		return EXIT_FAILURE;
	}
#endif // _VERBOSE
    wstring fileData = read_file_multi_byte(argv[1]);
	table_t *table;
    if (!(table = LoadTable(fileData.c_str()))) {
		fprintf(stderr, "%s not exists", argv[1]);
		return EXIT_FAILURE;
    }
#ifdef _VERBOSE
	int stdout_copy = dup(STDOUT_FILENO);
	freopen(argv[2], "w", stdout);
#ifdef _WIN32
	_setmode(STDOUT_FILENO, _O_U8TEXT);
#endif // _WIN32
#endif // _VERBOSE
	rule_list_t* rule_list = table_to_rules(table);
#ifdef _VERBOSE
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdout_copy);
	fflush(stdout);
#endif
	print_rules_db(table, rule_list);
	rule_list_free(rule_list);
    FreeTable(table);
	return EXIT_SUCCESS;
}