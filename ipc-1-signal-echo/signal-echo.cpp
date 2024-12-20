#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <pwd.h>
#include <sys/ucontext.h>
#include <unistd.h>

#define ERROR(fmt, ...)                                                        \
  do {                                                                         \
    fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);    \
    exit(1);                                                                   \
  } while (false)

void signal_handler(int _, siginfo_t *info, void *void_ctx) {
  struct passwd *passwd_data = getpwuid(info->si_uid);

  printf("Received a SIGUSR1 signal from process %d executed by %u "
         "(%s).\n",
         info->si_pid, info->si_uid, passwd_data->pw_name);

  greg_t *gregs = ((ucontext_t *)void_ctx)->uc_mcontext.gregs;
  printf("State of the context: RIP = %lld, RAX = %lld, RBX = %lld.\n",
         gregs[REG_RIP], gregs[REG_RAX], gregs[REG_RBX]);
}

int main() {
  struct sigaction action = {};
  action.sa_flags = SA_SIGINFO;
  action.sa_sigaction = signal_handler;

  if (sigaction(SIGUSR1, &action, nullptr) < 0) {
    ERROR("Failed to register signal handler: %s", strerror(errno));
  }

  pause();
}