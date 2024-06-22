#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runner.h"

void run()
{
        // Command to compile the generated C code
        const char *compileCommand = "gcc output.c -o output 2> gcc_output.log";

        // Run the compile command and capture the return status
        int compileStatus = system(compileCommand);

        // Check if the compilation was successful
        if (compileStatus != 0)
        {
                fprintf(stderr, "Failed to compile generated C code. See gcc_output.log for details.\n");

                // Open the log file and print its contents
                FILE *logFile = fopen("gcc_output.log", "r");
                if (logFile)
                {
                        char logLine[256];
                        while (fgets(logLine, sizeof(logLine), logFile))
                        {
                                fprintf(stderr, "%s", logLine);
                        }
                        fclose(logFile);
                }
                else
                {
                        fprintf(stderr, "Failed to open gcc_output.log for reading.\n");
                }

                exit(EXIT_FAILURE);
        }

        // Run the compiled program
        if (system("./output") != 0)
        {
                fprintf(stderr, "Failed to run compiled program\n");
                exit(EXIT_FAILURE);
        }
}
