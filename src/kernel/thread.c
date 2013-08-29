// 
// thread.c -- Defines functions and structures for multithreading.
//

#include <simple/thread.h>

thread_t *current_thread;
uint32_t next_tid = 0;

void thread_exit ();

extern void _create_thread(int (*)(void*), void*, uint32_t*, thread_t*);

thread_t *init_threading ()
{
  thread_t *thread = kmalloc (sizeof (thread_t));
  thread->id  = next_tid++;
  
  current_thread = thread;

  return thread;
}

thread_t *create_thread (int (*fn)(void*), void *arg, uint32_t *stack)
{
  thread_t *thread = kmalloc (sizeof (thread_t));
  memset (thread, 0, sizeof (thread_t));
  thread->id = next_tid++;
  
  *--stack = (uint32_t)arg;
  *--stack = (uint32_t)&thread_exit;
  *--stack = (uint32_t)fn;

  thread->esp = (uint32_t)stack;
  thread->ebp = 0;
  thread->eflags = 0x200; // Interrupts enabled.
  thread_is_ready(thread);

  return thread;
}

void thread_exit ()
{
  register uint32_t val asm ("eax");

  printk ("Thread exited with value %d\n", val);

  for (;;) ;
}
