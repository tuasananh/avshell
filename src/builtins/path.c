#include "path.h"

#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

#include "../dispatcher.h"

// Helper function to resolve relative paths to absolute paths
static LPWSTR resolve_to_absolute_path(LPCWSTR path) {
  if (path == NULL) {
    return NULL;
  }

  // GetFullPathNameW resolves relative and drive-relative paths (e.g., C:foo)
  // to a fully qualified absolute path.
  DWORD absolute_size = GetFullPathNameW(path, 0, NULL, NULL);
  if (absolute_size == 0) {
    return NULL;
  }

  LPWSTR absolute = (LPWSTR)malloc(absolute_size * sizeof(WCHAR));
  if (absolute == NULL) {
    return NULL;
  }

  // Retry if the required size changes between calls.
  while (true) {
    DWORD resolved_size =
        GetFullPathNameW(path, absolute_size, absolute, NULL);
    if (resolved_size == 0) {
      free(absolute);
      return NULL;
    }

    if (resolved_size < absolute_size) {
      return absolute;
    }

    DWORD new_size = resolved_size + 1;
    LPWSTR resized = (LPWSTR)realloc(absolute, new_size * sizeof(WCHAR));
    if (resized == NULL) {
      free(absolute);
      return NULL;
    }

    absolute = resized;
    absolute_size = new_size;
  }
}

static ExecutionResult path_handler(int argc, LPWSTR* argv) {
  if (argc != 1) {
    wprintf(L"Usage: %ls\n", argv[0]);
    return KEEP_RUNNING(EXIT_FAILURE);
  }

  DWORD buffer_size = GetEnvironmentVariableW(L"PATH", NULL, 0);
  if (buffer_size == 0) {
    wprintf(L"PATH environment variable is empty or not found.\n");
    return KEEP_RUNNING(0);
  }

  LPWSTR buffer = (LPWSTR)malloc(buffer_size * sizeof(WCHAR));
  if (buffer == NULL) {
    wprintf(L"Error: Not enough memory!\n");
    return KEEP_RUNNING(EXIT_FAILURE);
  }

  GetEnvironmentVariableW(L"PATH", buffer, buffer_size);
  wprintf(L"Current PATH: %ls\n", buffer);
  free(buffer);

  return KEEP_RUNNING(0);
}

Command path_command = {
    .name = L"path",
    .description = L"Displays the current PATH environment variable",
    .handler = path_handler};

static ExecutionResult addpath_handler(int argc, LPWSTR* argv) {
  if (argc != 2) {
    wprintf(L"Usage: %ls <directory>\n", argv[0]);
    return KEEP_RUNNING(EXIT_FAILURE);
  }

  // Resolve relative paths to absolute paths
  LPWSTR absolute_path = resolve_to_absolute_path(argv[1]);
  if (absolute_path == NULL) {
    wprintf(L"Error: Failed to resolve path '%ls'.\n", argv[1]);
    return KEEP_RUNNING(EXIT_FAILURE);
  }

  DWORD attribs = GetFileAttributesW(absolute_path);
  if (attribs == INVALID_FILE_ATTRIBUTES) {
    DWORD error = GetLastError();
    if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND) {
      wprintf(L"Error: The directory '%ls' does not exist.\n", absolute_path);
      free(absolute_path);
      return KEEP_RUNNING(EXIT_FAILURE);
    }

    ExecutionResult res =
        keep_running_with_error(argv[0], error, L"Failed to access directory");
    free(absolute_path);
    return res;
  }

  if (!(attribs & FILE_ATTRIBUTE_DIRECTORY)) {
    wprintf(L"Error: The path '%ls' is not a directory.\n", absolute_path);
    free(absolute_path);
    return KEEP_RUNNING(EXIT_FAILURE);
  }

  DWORD old_size = GetEnvironmentVariableW(L"PATH", NULL, 0);
  DWORD new_dir_len = wcslen(absolute_path);
  DWORD new_size = (old_size > 0 ? old_size : 1) + new_dir_len + 1;

  LPWSTR new_path = (LPWSTR)malloc(new_size * sizeof(WCHAR));
  if (new_path == NULL) {
    wprintf(L"Error: Not enough memory!\n");
    free(absolute_path);
    return KEEP_RUNNING(EXIT_FAILURE);
  }

  if (old_size > 0) {
    GetEnvironmentVariableW(L"PATH", new_path, old_size);

    LPWSTR found = wcsstr(new_path, absolute_path);
    if (found != NULL) {
      size_t len = wcslen(absolute_path);
      if ((found == new_path || found[-1] == L';') &&
          (found[len] == L';' || found[len] == L'\0')) {
        wprintf(L"Notice: The directory is already in PATH.\n");
        free(new_path);
        free(absolute_path);
        return KEEP_RUNNING(0);
      }
    }

    wcscat(new_path, L";");
    wcscat(new_path, absolute_path);
  } else {
    wcscpy(new_path, absolute_path);
  }

  if (!SetEnvironmentVariableW(L"PATH", new_path)) {
    ExecutionResult res = keep_running_with_error(argv[0], GetLastError(),
                                                  L"Failed to update PATH");
    free(new_path);
    free(absolute_path);
    return res;
  }

  free(new_path);
  free(absolute_path);
  wprintf(L"Success: Directory added to PATH.\n");
  return KEEP_RUNNING(0);
}

Command addpath_command = {
    .name = L"addpath",
    .description = L"Appends a new directory to the PATH variable",
    .handler = addpath_handler};
