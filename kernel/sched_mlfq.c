#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"

#define MLFQ_LEVELS 4
#define BOOST_TICKS 1000

static int mlfq_ticks_count = 0;
// Quantums for priority levels 0, 1, 2, 3
static const int quantums[MLFQ_LEVELS] = {1, 2, 4, 8};

void mlfq_init(void) {
  mlfq_ticks_count = 0;
}

void mlfq_proc_init(struct proc *p) {
  p->priority = 0;
  p->ticks_used = 0;
}

void mlfq_enqueue(struct proc *p) {}
void mlfq_dequeue(struct proc *p) {}

struct proc* mlfq_pick_next(void) {
  struct proc *p;
  // Iterate priority levels from highest (0) to lowest (3)
  for(int lvl = 0; lvl < MLFQ_LEVELS; lvl++) {
    for(p = proc; p < &proc[NPROC]; p++) {
      acquire(&p->lock);
      if(p->state == RUNNABLE && p->priority == lvl) {
        release(&p->lock);
        return p;
      }
      release(&p->lock);
    }
  }
  return 0;
}

void mlfq_tick(struct proc *p) {
  mlfq_ticks_count++;

  // Priority Boost: Reset all processes to top priority periodically
  if(mlfq_ticks_count >= BOOST_TICKS) {
    mlfq_ticks_count = 0;
    struct proc *pr;
    for(pr = proc; pr < &proc[NPROC]; pr++) {
      acquire(&pr->lock);
      if(pr->state != UNUSED) {
        pr->priority = 0;
        pr->ticks_used = 0;
      }
      release(&pr->lock);
    }
  }

  // Demote CPU-bound processes when quantum expires
  if(p && p->state == RUNNING) {
    p->ticks_used++;
    if(p->ticks_used >= quantums[p->priority]) {
      p->ticks_used = 0;
      if(p->priority < MLFQ_LEVELS - 1) {
        p->priority++;
      }
      yield();
    }
  }
}

struct sched_ops mlfq_ops = {
  .name = "MLFQ",
  .init = mlfq_init,
  .proc_init = mlfq_proc_init,
  .enqueue = mlfq_enqueue,
  .dequeue = mlfq_dequeue,
  .pick_next = mlfq_pick_next,
  .tick = mlfq_tick
};
