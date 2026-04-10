#include "dispatcher.h"

#include <assert.h>
#include <wchar.h>

#include "command.h"

ExecutionResult dispatch_command(int argc, LPWSTR* argv) {
  assert(argc > 0);

  for (size_t i = 0; i < n_commands; i++) {
    if (wcscmp(argv[0], commands[i]->name) == 0) {
      return commands[i]->handler(argc, argv);
    }
  }

  wprintf(L"Command not recognized: %ls\n", argv[0]);
  return KEEP_RUNNING(EXIT_FAILURE);
}

ExecutionResult keep_running_with_error(LPWSTR command, DWORD error_code,
                                        LPWSTR message) {
  if (message == NULL) {
    message = L"An unexpected error occurred";
  }
  wprintf(L"%ls: Error %zd: %ls.\n", command, error_code, message);
  return KEEP_RUNNING(EXIT_FAILURE);
}
