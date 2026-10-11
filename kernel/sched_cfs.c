#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"

void cfs_init(void) {}

void cfs_proc_init(struct proc *p) {
  p->vruntime = 0;
  p->nice = 0;
}

void cfs_enqueue(struct proc *p) {}
void cfs_dequeue(struct proc *p) {}

struct proc* cfs_pick_next(void) {
  struct proc *p;
  struct proc *min_p = 0;
  uint64 min_vruntime = ~0ULL; // Maximum uint64 value

  // Select process with the smallest vruntime
  for(p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);
    if(p->state == RUNNABLE) {
      if(p->vruntime < min_vruntime) {
        min_vruntime = p->vruntime;
        min_p = p;
      }
    }
    release(&p->lock);
  }
  return min_p;
}

void cfs_tick(struct proc *p) {
  if(p && p->state == RUNNING) {
    // Increase vruntime based on nice weight factor
    uint64 weight = (p->nice > 0) ? (p->nice + 1) : 1;
    p->vruntime += weight;
    yield();
  }
}

struct sched_ops cfs_ops = {
  .name = "CFS",
  .init = cfs_init,
  .proc_init = cfs_proc_init,
  .enqueue = cfs_enqueue,
  .dequeue = cfs_dequeue,
  .pick_next = cfs_pick_next,
  .tick = cfs_tick
};
