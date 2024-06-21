#include <stdio.h>
#include <stdlib.h>

char *readSourceFromFile(const char *filename)
{
        FILE *file = fopen(filename, "r");
        if (!file)
        {
                fprintf(stderr, "Could not open file %s\n", filename);
                exit(EXIT_FAILURE);
        }

        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *buffer = malloc(length + 1);
        if (!buffer)
        {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
        }

        size_t source_code_reading = fread(buffer, 1, length, file);
        buffer[length] = '\0';

        fclose(file);
        printf("File contents %s", buffer);
        return buffer;
}

int main()
{
        char *sourceCode = readSourceFromFile("test/test_source.txt");

        return EXIT_SUCCESS;
}
