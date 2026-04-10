#include "command.h"

#include "exit.h"
#include "help.h"

Command* commands[] = {
    &help_command, &exit_command
    // Add more commands here
};

const size_t n_commands = sizeof(commands) / sizeof(commands[0]);
