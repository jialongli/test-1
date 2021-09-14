#include <stdio.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/prctl.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>

extern int errno;

enum {
	BREAK_EXEC = 0x0,
	BREAK_WRITE = 0x1,
	BREAK_READWRITE = 0x3,
};

enum {
	BREAK_ONE = 0x0,
	BREAK_TWO = 0x1,
	BREAK_FOUR = 0x3,
	BREAK_EIGHT = 0x2,
};

#define ENABLE_BREAKPOINT(x) (0x1<<(x*2))
#define ENABLE_BREAK_EXEC(x) (BREAK_EXEC<<(16+(x*4)))
#define ENABLE_BREAK_WRITE(x) (BREAK_WRITE<<(16+(x*4)))
#define ENABLE_BREAK_READWRITE(x) (BREAK_READWRITE<<(16+(x*4)))

/*
 * This function fork()s a child that will use
 * ptrace to set a hardware breakpoint for 
 * memory r/w at _addr_. When the breakpoint is
 * hit, then _handler_ is invoked in a signal-
 * handling context.
 */
bool install_breakpoint(void *addr, int bpno, void (*handler)(int)) {
	pid_t child = 0;
	uint32_t enable_breakpoint = ENABLE_BREAKPOINT(bpno);
	uint32_t enable_breakwrite = ENABLE_BREAK_WRITE(bpno);
	pid_t parent = getpid();
	int child_status = 0;

	if (!(child = fork())) //子进程
	{
		int parent_status = 0;
		if (ptrace(PTRACE_ATTACH, parent, NULL, NULL))
			_exit(1);

		while (!WIFSTOPPED(parent_status))
			waitpid(parent, &parent_status, 0);
	
		/*
		 * set the breakpoint address.
		 */
		if (ptrace(PTRACE_POKEUSER,
		           parent,
		           offsetof(struct user, u_debugreg[bpno]),
		           addr))
			_exit(1);

		/*
		 * set parameters for when the breakpoint should be triggered.
		 */
		if (ptrace(PTRACE_POKEUSER,
		           parent,
		           offsetof(struct user, u_debugreg[7]),
		           enable_breakwrite | enable_breakpoint))
			_exit(1);

		if (ptrace(PTRACE_DETACH, parent, NULL, NULL))
			_exit(1);

		_exit(0);
	}

	waitpid(child, &child_status, 0);

    //---------------------------------------------------------------------------------------------
    //在大多数系统，gdb对使用fork创建的进程没有进行特别的支持。当父进程使用fork创建子进程
    //，gdb仍然只会调试父进程，而子进程没有得到控制和调试。这个时候，如果你在子进程执行到
    //的代码中设置了断点，那么当子进程执行到这个断点的时候，会产生一个SIGTRAP的信号，
    //如果没有对此信号进行捕捉处理，就会按默认的处理方式处理——终止进程。
	signal(SIGTRAP, handler);

	if (WIFEXITED(child_status) && !WEXITSTATUS(child_status))
		return true;
	return false;
}

/*
 * This function will disable a breakpoint by
 * invoking install_breakpoint is a 0x0 _addr_
 * and no handler function. See comments above
 * for implementation details.
 */
bool disable_breakpoint(int bpno) 
{
	return install_breakpoint(0x0, bpno, NULL);
}

/*
 * Example of how to use this /library/.
 */
int handled = 0;

void handle(int s) {
	handled += 1;
	return;
}

int main(int argc, char **argv) {
	int i, a = 0;

	if (!install_breakpoint(&a, 0, handle))
		printf("failed to set the breakpoint!\n");

    for(i=0;i<100202;i++) {
        a = 1;
    }

	printf("handled: %d\n", handled);

	if (!disable_breakpoint(0))
		printf("failed to disable the breakpoint!\n");



	return 1;
}

