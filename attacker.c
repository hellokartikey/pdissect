#include <sys/ptrace.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

/*
long ptrace(
	enum __ptrace_request 	request, 
	pid_t 			pid,
        void*			addr, 
	void*			data
);
*/

void my_error();
void attach(pid_t pid);
void detach(pid_t pid);
void peekdata(pid_t pid, uint64_t addr, uint64_t* data);
void pokedata(pid_t pid, uint64_t addr, uint64_t data);

void my_error() {
	perror("[-] ptrace");
	exit(errno);
}

void attach(pid_t pid) {
	printf("[*] attaching...\n");
	if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) != 0) {
		my_error();
	}
	printf("[+] successfully attached to pid %d\n", pid);
}

void detach(pid_t pid) {
	printf("[*] detaching...\n");
	if (ptrace(PTRACE_DETACH, pid, NULL, NULL) != 0) {
		my_error();
	}
	printf("[+] successfully detached from pid %d\n", pid);
}

void peekdata(pid_t pid, uint64_t addr, uint64_t* data) {
	printf("[*] peeking...\n");
	*data = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
	if (*data < 0) {
		my_error();
	}
	printf("[+] data: %lu\n", *data);
}

void pokedata(pid_t pid, uint64_t addr, uint64_t data) {
	printf("[*] trying to alter value at %lx to %lu...\n", addr, &data);
	if (ptrace(PTRACE_POKEDATA, pid, addr, data) != 0) {
		my_error();
	}
	printf("[+] successfully altered memory\n");
}

int main(int argc, char* argv[]) {

	if (argc != 4) {
		fprintf(stderr, "Usage: %s <pid> <addr> <new_val>\n", argv[0]);
		exit(1);
	}

	pid_t pid = atoi(argv[1]);
	uint64_t addr = strtol(argv[2], NULL, 16);
	uint32_t new = atoi(argv[3]);

	printf("[+] pid: %d\n", pid);
	printf("[+] addr: %lx\n", addr);

	// attach
	attach(pid);

	// peak_data
	uint64_t data;
	peekdata(pid, addr, &data);

	uint32_t lower, upper;
	lower = data;
	upper = data >> 32;
	printf("[+] upper: %d\n", upper);
	printf("[+] lower: %d\n", lower);

	uint64_t write = upper;
	write = (write << 32) + new;
	
	// poke_data
	pokedata(pid, addr, write);

	// detach
	detach(pid);
	
	return 0;
}
