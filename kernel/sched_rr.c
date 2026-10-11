#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"

void rr_init(void) {}
void rr_proc_init(struct proc *p) {}
void rr_enqueue(struct proc *p) {}
void rr_dequeue(struct proc *p) {}

struct proc* rr_pick_next(void) {
  struct proc *p;
  // Search process table for the next RUNNABLE process
  for(p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);
    if(p->state == RUNNABLE) {
      release(&p->lock);
      return p;
    }
    release(&p->lock);
  }
  return 0;
}

void rr_tick(struct proc *p) {
  if(p && p->state == RUNNING) {
    yield();
  }
}

struct sched_ops rr_ops = {
  .name = "Round-Robin",
  .init = rr_init,
  .proc_init = rr_proc_init,
  .enqueue = rr_enqueue,
  .dequeue = rr_dequeue,
  .pick_next = rr_pick_next,
  .tick = rr_tick
};

struct sched_ops *current_sched = &rr_ops;
