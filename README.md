# SchedForge — Pluggable CPU Schedulers in xv6-riscv

SchedForge extends the **xv6-riscv** operating system with a modular, pluggable CPU scheduling architecture (`struct sched_ops`). It allows for dynamic policy switching at runtime between Round-Robin, Multi-Level Feedback Queue (MLFQ), and Completely Fair Scheduler (CFS) algorithms.

## Features

* **Abstract Scheduler Interface (`struct sched_ops`)**: Standardized abstraction layer using function pointers (`init`, `proc_init`, `enqueue`, `dequeue`, `pick_next`, `tick`).
* **Pluggable Scheduling Modules**:
  * `sched_rr.c`: Round-Robin scheduling (default xv6 behavior).
  * `sched_mlfq.c`: Multi-Level Feedback Queue with priority levels, quantum scaling, and starvation prevention boosting.
  * `sched_cfs.c`: Completely Fair Scheduler tracking virtual runtime (`vruntime`) and nice weighting.
* **Dynamic Runtime Switching**: `setscheduler(int policy)` system call (`SYS_setscheduler`) to swap active scheduler policies dynamically.
* **Benchmarking Utility**: `schedtest` user-space program to validate policy switches and benchmark CPU-bound process execution.

## Build & Run

Compile and boot the kernel using QEMU:
```bash
make clean && make qemu
