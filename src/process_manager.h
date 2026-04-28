#pragma once

#include <stddef.h>
#include <wchar.h>
#include <windows.h>

extern volatile DWORD g_foreground_process_pid;

typedef enum { STATUS_RUNNING, STATUS_STOPPED } ProcessStatus;

typedef struct {
  DWORD pid;
  HANDLE handle;
  WCHAR command[256];
  ProcessStatus status;
} ProcessInfo;

void process_manager_init(void);
void process_manager_cleanup(void);
ProcessInfo* process_manager_get(size_t index);
size_t process_manager_count(void);

void add_background_process(DWORD pid, HANDLE handle, LPCWSTR command);
void remove_background_process(DWORD pid);
int find_process_index(DWORD pid);
void clean_dead_processes(void);