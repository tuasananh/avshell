#include "list.h"

#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#include "../dispatcher.h"
#include "../process_manager.h"

static ExecutionResult handler(int argc, LPWSTR* argv) {
  if (argc != 1) {
    wprintf(L"Usage: %ls\n", argv[0]);
    return KEEP_RUNNING(EXIT_FAILURE);
  }

  clean_dead_processes();

  if (process_manager_count() == 0) {
    wprintf(L"No background processes running.\n");
    return KEEP_RUNNING(0);
  }

  wprintf(L"%-10s %-25s %-15s\n", L"PID", L"Command", L"Status");
  wprintf(L"--------------------------------------------------\n");

  for (size_t i = 0; i < process_manager_count(); i++) {
    ProcessInfo* p = process_manager_get(i);
    wprintf(L"%-10lu %-25ls %-15ls\n", p->pid, p->command,
            p->status == STATUS_RUNNING ? L"Running" : L"Stopped");
  }

  return KEEP_RUNNING(0);
}

Command list_command = {.name = L"list",
                        .description = L"Lists all background processes",
                        .handler = handler};
