#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <value>\n", argv[0]);
		exit(1);
	}

	printf("starting victim process...\n");

	int value = atoi(argv[1]);
	int count = 70;
	printf("address of count %p\n", &count);
	while (1) {
		printf("#%04d (%d) %p : %d\n", count, getpid(), &value, value);
		sleep(1);
		count++;
	}

	return 0;
}
