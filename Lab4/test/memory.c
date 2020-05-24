// memory.c
// Проверка функций malloc, calloc, free

#include <stdio.h>
#include <stdlib.h>

int main() {
	void* ptr = malloc(0);
	printf("malloc(0) = %016X\n", ptr);
	free(ptr);
	
	ptr = calloc(0, sizeof(int));
	printf("calloc(0, int) = %016X\n", ptr);
	free(ptr);
	
	// Nothing to be done
	ptr = NULL;
	free(ptr);
	
	return 0;
}