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
        for (int i = 0; i < root->numChildren && root->children[i] != NULL; i++)
        {
                Node *child = root->children[i];
                if (child == NULL)
                {
                        printf("Broke from codegen loop");
                        break;
                }
                if (child->type == DECLARATION_NODE)
                {

                        // Add condition to check what kind of literal is it ,  int bool etc (for future additions)
                        printf("%s %s = %s;\n", "int", child->value, child->children[0]->value);
                        fprintf(output, "%s %s;\n", "int", child->value);
                }
                else if (child->type == ASSIGNMENT_NODE)
                {
                        if (child->children[0]->type == BIN_OP_NODE)
                        {
                                //  Add code for if there are more than two operands
                                Node *operator= child->children[0];
                                Node *left_operand = operator->children[0];
                                Node *right_operand = operator->children[1];
                                fprintf(output, "%s =%s %s %s;\n", child->value, left_operand->value, operator->value, right_operand->value);
                        }
                        else
                        {
                                fprintf(output, "%s = %s;\n", child->value, child->children[0]->value);
                        }
                }
                else if (child->type == PRINT_STATEMENT_NODE)
                {
                        fprintf(output, "printf(\"%%d\\n\", %s);\n", child->children[0]->value);
                }
                else if (child->type == DECLARATION_ASSIGNMENT_NODE)
                {
                        // Add condition to check what kind of literal is it ,  int bool etc (for future additions)
                        if (child->children[0]->type == BIN_OP_NODE)
                        {
                                //  Add code for if there are more than two operands
                                Node *operator= child->children[0];
                                Node *left_operand = operator->children[0];
                                Node *right_operand = operator->children[1];
                                fprintf(output, "%s %s =%s %s %s;\n", "int", child->value, left_operand->value, operator->value, right_operand->value);
                        }
                        else
                        {
                                fprintf(output, "%s %s = %s;\n", "int", child->value, child->children[0]->value);
                        }
                }
        }
        fprintf(output, "return 0;\n}");
        fflush(output); // Ensure the data is written to disk
        fclose(output);
}
