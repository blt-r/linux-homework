#include <format>
#include <string_view>
#include <vector>

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERROR(fmt, ...)                                                        \
  do {                                                                         \
    fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);    \
    exit(1);                                                                   \
  } while (false)

int do_command(const char *name, char *const args[]) {
  int pid = fork();
  if (pid < 0) {
    ERROR("%s", strerror(errno));
  }

  if (pid == 0) {
    execvp(name, args);
    ERROR("%s", strerror(errno));
  }

  int status;

  if (waitpid(pid, &status, 0) < 0) {
    ERROR("%s", strerror(errno));
  }

  return WEXITSTATUS(status);
}

int do_command_silent(const char *name, char *const args[]) {
  pid_t pid = fork();
  if (pid < 0) {
    ERROR("%s", strerror(errno));
  }

  if (pid == 0) {
    pid_t my_pid = getpid();
    std::string filename = std::format("{}.log", my_pid);

    if (close(STDOUT_FILENO) < 0) {
      ERROR("%s", strerror(errno));
    }

    if (creat(filename.c_str(), 0644) < 0) {
      ERROR("%s", strerror(errno));
    }

    execvp(name, args);
    ERROR("%s", strerror(errno));
  }

  int status;

  if (waitpid(pid, &status, 0) < 0) {
    ERROR("%s", strerror(errno));
  }

  return WEXITSTATUS(status);
}

bool is_whitespace(char ch) {
  return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
}

void parse_command(std::vector<char *> &args, std::string_view str) {
  for (char *arg : args) {
    delete[] arg;
  }
  args.clear();

  size_t word_begin = 0;

  while (word_begin < str.size()) {
    while (word_begin < str.size() && is_whitespace(str[word_begin])) {
      ++word_begin;
    }

    if (word_begin == str.size()) {
      break;
    }

    size_t word_end = word_begin;
    while (word_end < str.size() && !is_whitespace(str[word_end])) {
      ++word_end;
    }

    size_t len = word_end - word_begin;
    char *arg = new char[len + 1];
    arg[len] = '\0';
    std::memcpy(arg, str.data() + word_begin, len);

    args.push_back(arg);
    word_begin = word_end;
  }

  args.push_back(nullptr);
}

const char *starts_with(const char *haystack, const char *needle) {
  if (haystack == nullptr) {
    return nullptr;
  }

  while (*haystack && *needle) {
    if (*haystack != *needle) {
      return nullptr;
    }
    ++haystack;
    ++needle;
  }

  if (*needle != '\0') {
    return nullptr;
  }

  return haystack;
}

#define ANSI_GREEN_FG "\e\[32m"
#define ANSI_RESET_FG "\e\[m"

void prompt() {
  if (!isatty(STDIN_FILENO)) {
    return;
  }

  char *pwd = get_current_dir_name();
  char *home = getenv("HOME");

  const char *cut_at_home = starts_with(pwd, home);
  if (cut_at_home) {
    printf(ANSI_GREEN_FG "~%s" ANSI_RESET_FG " > ", cut_at_home);
  } else {
    printf(ANSI_GREEN_FG "%s" ANSI_RESET_FG " > ", pwd);
  }

  fflush(stdout);

  free(pwd);
}

void cd(char *dir) {
  if (dir == nullptr) {
    dir = getenv("HOME");
    if (dir == nullptr) {
      fprintf(stderr, "Error: $HOME is unset\n");
    }
  }

  int error = chdir(dir);

  if (error == -1) {
    fprintf(stderr, "Error: %s\n", strerror(errno));
  }
}

void exit_from_shell(char *exit_code_str) {
  int status = 0;

  if (exit_code_str != nullptr) {
    char *end;
    status = strtol(exit_code_str, &end, 0);
    if (*end) {
      fprintf(stderr, "Invalid exit code\n");
      return;
    }
  }

  exit(status);
}

int main(void) {
  char *line = nullptr;
  size_t len = 0;
  std::vector<char *> args;

  while (true) {
    prompt();
    ssize_t read_n = getline(&line, &len, stdin);
    if (read_n == -1) {
      break;
    }

    parse_command(args, std::string_view(line, read_n));

    if (args.size() == 1) { // there's always a trailing nullptr
      continue;
    }

    if (strcmp(args[0], "silent") == 0) {
      if (args.size() < 3) {
        fprintf(stderr, "Provide command to execute silently\n");
        continue;
      }
      do_command_silent(args[1], args.data() + 1);
    } else if (strcmp(args[0], "cd") == 0) {
      if (args.size() > 3) {
        fprintf(stderr, "To many args for cd command\n");
        continue;
      }
      cd(args[1]); // nullptr if cd was called without args
    } else if (strcmp(args[0], "exit") == 0) {
      if (args.size() > 3) {
        fprintf(stderr, "To many args for exit command\n");
        continue;
      }
      exit_from_shell(args[1]);
    } else if (args.size() > 1) {
      do_command(args[0], args.data());
    }
  }

  for (char *arg : args) {
    delete[] arg;
  }
  free(line);
}
