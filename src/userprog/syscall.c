#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "filesys/file.h"
#include "filesys/filesys.h"

static void syscall_handler (struct intr_frame *);
void get_args_from_stack(const void *esp, char *argv, int count);
bool verify_ptr(const void *vaddr);

void syscall_init (void) {
  lock_init(&filesys_lock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void syscall_handler (struct intr_frame *f)  {
  // Get stack pointer
  int *esp = f->esp;

  // Arguments from stack
  char argv[3];

  switch(*(int*)esp) {
    case SYS_HALT:
      printf("SYSCALL_HALT\n");
      syscall_halt();
      break;
    case SYS_EXIT:
      if(!verify_ptr((void*)(esp + 1)))
        syscall_exit(-1);
      syscall_exit((int)*(esp+1));
      break;
    case SYS_EXEC:
      printf("[SYSCALL EXEC]\n");
      break;
    case SYS_WAIT:
      printf("[SYSCALL WAIT]\n");
      break;
    case SYS_CREATE:
      printf("[SYSCALL CREATE]\n");
      break;
    case SYS_REMOVE:
      printf("[SYSCALL REMOVE]\n");
      break;
    case SYS_OPEN:
      printf("[SYSCALL OPEN]\n");
      break;
    case SYS_FILESIZE:
      printf("[SYSCALL FILESIZE]\n");
      break;
    case SYS_READ:
      printf("[SYSCALL READ]\n");
      break;
    case SYS_WRITE:
      printf("[SYSCALL WRITE]\n");
      if(verify_ptr((const void*)(esp+5)) && verify_ptr( (const void*) (esp+6)) && verify_ptr((const void*)(esp+7)))
      {
        if(verify_ptr((const void*)(*(esp+6))) && verify_ptr((const void*)((*(esp+6)+*(esp+7)-1))))
          f->eax = (uint32_t) syscall_write((int) *(esp+5), (const void*) *(esp+6), (unsigned) *(esp+7));
        else{
          syscall_exit(-1);
        }
      }else{
        syscall_exit(-1);
      }
      break;
    case SYS_SEEK:
      printf("[SYSCALL SEEK]\n");
      break;
    case SYS_TELL:
      printf("[SYSCALL TELL]\n");
      break;
    case SYS_CLOSE:
      printf("[SYSCALL CLOSE]\n");
      break;
  }
}
/* Terminates Pintos by calling shutdown_power_off() (declared in threads/init.h). 
 * This should be seldom used, because you lose some information about possible deadlock situations, etc.
 */
void syscall_halt(void) {
  printf("[SYSCALL HALT]\n");

  shutdown_power_off();
}

/* Terminates the current user program, returning status to the kernel. 
 * If the process's parent waits for it (see below), this is the status that will be returned. 
 * Conventionally, a status of 0 indicates success and nonzero values indicate errors.
 */
void syscall_exit(int status) {
  printf("[SYSCALL EXIT] Thread %s: status %d\n", thread_current()->name, status);

  thread_exit();
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
  if(size <= 0) {
    return -1;
  }
  if(fd == STDIN_FILENO){
    return -1;
  }
  if(fd == STDOUT_FILENO){
    putbuf (buffer, size);
    return size;
  }

  // write to file
  lock_acquire(&filesys_lock);
  struct file *fd_struct = getFile(fd);

  if(fd_struct == NULL) {
    lock_release(&filesys_lock);
    return -1;
  }

  int bytes_written = file_write(fd_struct, buffer, size);
  lock_release(&filesys_lock);
  return bytes_written;
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

struct file *getFile(int fd) {
  struct thread *td = thread_current();
  struct list_elem *list;
  struct file_descriptor *f_descriptor;

  for(list = list_begin(&td->file_list); list != list_end(&td->file_list); list = list_next(list)) {
    f_descriptor = list_entry(list, struct file_descriptor, elem);
    if (fd == f_descriptor->fd) {
      return f_descriptor->file;
    }
  }
  return NULL;
}

// Get arguments stored in stack
void get_args_from_stack(const void *esp, char *argv, int count) {
  int *esp_ptr;
  for(int i = 0; i < count; i++) {
    esp_ptr = (int*)esp + i + 1;
    if(!verify_ptr((const void*) esp_ptr))
      syscall_exit(-1);
    argv[i] = *esp_ptr;
  }
}

bool verify_ptr(const void *vaddr) {
  // Check to make sure address is not a null pointer
  bool isNullAddr = vaddr == NULL;

  // Check if address is pointer to kernel virtual address space
  bool isKernelSpace = is_kernel_vaddr(vaddr);

  if(isNullAddr) {
    printf("INVALID POINTER: Null Pointer\n");
    return false;
  }
  else if(isKernelSpace) {
    printf("INVALID POINTER: Pointer to kernel virtual address space\n");
    return false;
  } else {
    // Check if address is pointer to unmapped virtual memory
    struct thread *td = thread_current();
    bool isUaddrMapped = pagedir_get_page(td->pagedir, vaddr) != NULL; // pagedir_get_page returns NULL if UADDR is unmapped
    if(!isUaddrMapped) {
      printf("INVALID POINTER: Unmapped to virtual memory\n");
      return false;
    }
  }
  return true;
}
