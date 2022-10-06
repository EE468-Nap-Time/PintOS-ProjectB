#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"

static void syscall_handler (struct intr_frame *);
void get_args(struct intr_frame *f, char *argv, int count);
bool verify_ptr(const void *vaddr);

void syscall_init (void) {
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  printf("SYSCALL INIT\n");
}

static void syscall_handler (struct intr_frame *f)  {
  printf ("SYSCALL HANDLER!\n");

  // Get stack pointer
  void *esp = f->esp;

  switch(*(int*)esp) {
    case SYS_HALT:
      printf("SYSCALL_HALT\n");
      syscall_halt();
      break;
    case SYS_EXIT:
      break;
    case SYS_EXEC:
      break;
    case SYS_WAIT:
      break;
    case SYS_CREATE:
      break;
    case SYS_REMOVE:
      break;
    case SYS_OPEN:
      break;
    case SYS_FILESIZE:
      break;
    case SYS_READ:
      break;
    case SYS_WRITE:
      break;
    case SYS_SEEK:
      break;
    case SYS_TELL:
      break;
    case SYS_CLOSE:
      break;
  }

  // thread_exit();
}
/* Terminates Pintos by calling shutdown_power_off() (declared in threads/init.h). 
 * This should be seldom used, because you lose some information about possible deadlock situations, etc.
 */
void syscall_halt(void) {
  shutdown_power_off();
}

/* Terminates the current user program, returning status to the kernel. 
 * If the process's parent waits for it (see below), this is the status that will be returned. 
 * Conventionally, a status of 0 indicates success and nonzero values indicate errors.
 */
void syscall_exit(int status) {

}

/* Runs the executable whose name is given in cmd_line, passing any given arguments, 
 * and returns the new process's program id (pid). Must return pid -1, which otherwise 
 * should not be a valid pid, if the program cannot load or run for any reason. 
 * Thus, the parent process cannot return from the exec until it knows whether the child 
 * process successfully loaded its executable. You must use appropriate synchronization to ensure this.
 */
pid_t syscall_exec(const char *cmd_line) {

}

/* Waits for a child process pid and retrieves the child's exit status. */
int syscall_wait(pid_t pid) {

}

/* Creates a new file called file initially initial_size bytes in size. 
 * Returns true if successful, false otherwise. Creating a new file does not open it: 
 * opening the new file is a separate operation which would require a open system call.
 */
bool syscall_create(const char *file, unsigned initial_size) {

}

/* Deletes the file called file. Returns true if successful, false otherwise. 
 * A file may be removed regardless of whether it is open or closed, and removing 
 * an open file does not close it.
 */
bool syscall_remove(const char *file) {

}

/* Opens the file called file. Returns a nonnegative integer handle called a 
 * "file descriptor" (fd), or -1 if the file could not be opened.
 */
int syscall_open(const char *file) {

}

/* Returns the size, in bytes, of the file open as fd. */
int syscall_filesize(int fd) {

}

/* Reads size bytes from the file open as fd into buffer. Returns the number 
 * of bytes actually read (0 at end of file), or -1 if the file could not be read 
 * (due to a condition other than end of file). Fd 0 reads from the keyboard using input_getc().
 */
int syscall_read(int fd, void *buffer, unsigned size) {

}

/* Writes size bytes from buffer to the open file fd. Returns the number of bytes 
 * actually written, which may be less than size if some bytes could not be written.
 */
int syscall_write(int fd, const void *buffer, unsigned size) {

}

/* Changes the next byte to be read or written in open file fd to position, 
 * expressed in bytes from the beginning of the file. (Thus, a position of 0 is the file's start.)
 */
void syscall_seek(int fd, unsigned position) {

}

/* Returns the position of the next byte to be read or written in open file fd, 
 * expressed in bytes from the beginning of the file.
 */
unsigned syscall_tell(int fd) {

}

/* Closes file descriptor fd. Exiting or terminating a process implicitly closes all its 
 * open file descriptors, as if by calling this function for each one.
 */
void syscall_close(int fd) {

}

// Get arguments stored in stack
void get_args(struct intr_frame *f, char *argv, int count) {
  for(size_t i = 0; i < count; i++) {

  }
}

bool verify_ptr(const void *vaddr) {
  // Check to make sure address is not a null pointer
  bool isNullAddr = fault_addr == NULL;

  // Check if address is pointer to kernel virtual address space
  bool isKernelSpace = is_kernel_vaddr(fault_addr);

  if(isNullAddr || isKernelSpace) {
    printf("Invalid Pointer\n");
    return false;
  } else {
    // Check if address is pointer to unmapped virtual memory
    struct thread *td = thread_current();
    bool isUaddrMapped = pagedir_get_page(td->pagedir, fault_addr) != NULL; // pagedir_get_page returns NULL if UADDR is unmapped
    if(isUaddrMapped) {
      printf("Invalid Pointer\n");
      return false;
    }
  }
  return true;
}
