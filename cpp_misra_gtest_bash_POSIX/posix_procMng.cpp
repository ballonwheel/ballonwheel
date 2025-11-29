

/* work child processes, system calls, and process IDs */

/* explain tion of fork() shortly, pros and cons */
/* the fork() system call is used to create a new process by duplicating the calling process. 
   The new process is referred to as the child process. 
   The child process gets a unique process ID (PID) and has its own memory space.
   The fork() call returns a value of 0 to the child process and returns the child's PID to the parent process.
   Pros: 
   - Simple way to create new processes
   - Child process can run concurrently with the parent
   Cons:
   - Can lead to resource exhaustion if too many processes are created
   - Requires careful management of shared resources between parent and child processes
*/


/*  simple code demonstrating POSIX fork () usage */
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* fork */
void fork_example() {
    pid_t pid = fork();

    if (pid < 0) {
	std::cerr << "Fork failed!" << std::endl;
    } else if (pid == 0) {
	// Child process
	std::cout << "Hello from the child process! My PID is " << getpid() << std::endl;
    } else {
	// Parent process
	std::cout << "Hello from the parent process! My PID is " << getpid() << " and my child's PID is " << pid << std::endl;
	wait(NULL); // Wait for child process to finish
    }
}

int main() {
    fork_example();
    return 0;
}


/* other child process creation then fork() is vfork() and posix_spawn() */
/* in summary tey are more efficient in certain scenarios, especially when the child process will immediately call exec() to run a new program. 
   vfork() is similar to fork() but is optimized for this use case, as it does not duplicate the parent's address space.
   posix_spawn() is a higher-level function that combines process creation and execution in one call, providing more control over the new process's attributes.
*/


/* exec */

/* getpid */

/* wait */





