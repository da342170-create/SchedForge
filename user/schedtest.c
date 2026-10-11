#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void cpu_work(void) {
  volatile int x = 0;
  for (int i = 0; i < 50000000; i++) {
    x += (i * 3) / 2;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: schedtest <0: RR | 1: MLFQ | 2: CFS>\n");
    exit(1);
  }

  int sched_id = atoi(argv[1]);

  if (setscheduler(sched_id) < 0) {
    printf("Error: Failed to switch scheduler policy.\n");
    exit(1);
  }

  printf("Successfully switched scheduler policy to %d!\n", sched_id);

  // Fork child processes to simulate CPU load
  for (int i = 0; i < 3; i++) {
    int pid = fork();
    if (pid == 0) {
      cpu_work();
      printf("Child %d finished CPU task.\n", getpid());
      exit(0);
    }
  }

  for (int i = 0; i < 3; i++) {
    wait(0);
  }

  printf("Scheduler benchmark complete.\n");
  exit(0);
}
