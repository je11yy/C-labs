#include "tree.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4,
    runtime_error = -5,
    invalid_brackets = -6,
    invalid_arguments = -7,
    delete_file_error = -8
};

void print_tree(Node_ptr root, int level) 
{
    if (!root) return;
    print_tree(root -> left, level + 1);
    for (int i = 0; i < level; ++i) printf("  ");
    printf("%c\n", root -> data);
    print_tree(root -> right, level + 1);
}

void print_infix(FILE * file, Node_ptr root)
{
    if (!root) return;
    int flag = 0;
    if (is_operator(root -> data) == success) fprintf(file, "(");
    if (root -> data == '~')
    {
        flag = 1;
        fprintf(file, "%c", root -> data);

    }
    print_infix(file, root -> left);
    if (!flag)
    {
        fprintf(file, "%c", root -> data);
        if (root -> data == '-' || root -> data == '+' || root -> data == '<') fprintf(file, ">");
    }
    print_infix(file, root -> right);
    if (is_operator(root -> data) == success) fprintf(file, ")");
}

Node_ptr create_tree_node(char data, Node_ptr left, Node_ptr right)
{
    Node_ptr node = (Node_ptr)malloc(sizeof(Node));
    if (!node) return NULL;
    node -> data = data;
    node -> left = left;
    node -> right = right;
    return node;
}

void free_tree_node(Node_ptr node)
{
    node -> left = NULL;
    node -> right = NULL;
    free(node);
    node = NULL;
}

void free_tree(Node_ptr root)
{
    if (!root) return;
    free_tree(root -> left);
    free_tree(root -> right);
    free_tree_node(root);
}