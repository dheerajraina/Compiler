#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

void generate(Node *root)
{
        FILE *output = fopen("output.c", "w");
        if (output == NULL)
        {
                fprintf(stderr, "Failed to open output file\n");
                exit(EXIT_FAILURE);
        }
        printf("Generated Code:\n");
        fprintf(output, "#include<stdio.h>\n");
        fprintf(output, "int main(){\n");
        for (int i = 0; i < 2 && root->children[i] != NULL; i++)
        {
                Node *child = root->children[i];
                if (child == NULL)
                {
                        printf("Broke from codegen loop");
                        break;
                }
                if (child->type == DECLARATION)
                {
                        printf("%s %s = %s;\n", "int", child->value, child->children[0]->value);
                        fprintf(output, "%s %s = %s;\n", "int", child->value, child->children[0]->value);
                }
                else if (child->type == ASSIGNMENT)
                {
                        printf("%s = %s;\n", child->value, child->children[0]->value);
                        fprintf(output, "%s = %s;\n", child->value, child->children[0]->value);
                }
                else if (child->type == PRINT_STATEMENT)
                {
                        printf("printf(\"%%d\\n\", %s);\n", child->children[0]->value);
                        fprintf(output, "printf(\"%%d\\n\", %s);\n", child->children[0]->value);
                }
        }
        fprintf(output, "return 0;\n}");
        fflush(output); // Ensure the data is written to disk
        fclose(output);
}
