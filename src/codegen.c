#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

void writeIfConditional(Node *node, FILE *output)
{
        Node *test = node->children[0];
        Node *binOpNode = test->children[0];
        Node *ifBody = node->children[1];
        printf("------test node %s", binOpNode->value);
        char *if_elif = node->type == IF_STATEMENT_NODE ? "if" : "else if";
        fprintf(output, "%s(%s %s %s){", if_elif, binOpNode->children[0]->value, binOpNode->value, binOpNode->children[1]->value);
        generate(ifBody, output);
        fprintf(output, "}");

        if (node->numChildren == 3)
        {
                if (node->children[2]->type == ELIF_CLAUSE_NODE)
                {
                        printf("ELIF generate");
                        writeIfConditional(node->children[2], output);
                }
                else if (node->children[2]->type == ELSE_CLAUSE_NODE)
                {
                        Node *body = node->children[2]->children[0];
                        fprintf(output, "else{");
                        generate(body, output);
                        fprintf(output, "}");
                }
        }
}

void generate(Node *root, FILE *output)
{

        for (int i = 0; i < root->numChildren; i++)
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
                else if (child->type == FOR_LOOP_NODE)
                {
                        Node *init = child->children[0];
                        Node *fromExpr = init->children[0];
                        Node *toExpr = init->children[1];
                        Node *loopBody = child->children[1];
                        fprintf(output, "for (int %s=%s;%s<%s;%s++){\n", init->value, fromExpr->value, init->value, toExpr->value, init->value);
                        generate(loopBody, output);
                        fprintf(output, "}");
                }
                else if (child->type == IF_STATEMENT_NODE)
                {
                        writeIfConditional(child, output);
                }
        }
}

void codeGenerator(Node *root)
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
        generate(root, output);
        fprintf(output, "return 0;\n}");
        fflush(output); // Ensure the data is written to disk
        fclose(output);
}
