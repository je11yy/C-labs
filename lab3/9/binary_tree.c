#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "binary_tree.h"
#include "string.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4
};

int get_depth(Node_ptr node)
{
    if (!(node -> parent)) return 0;
    return (1 + get_depth(node -> parent));
}

void print_tree(Node_ptr root, int space)
{
    if (!root) return;
    if (root -> left) print_tree(root -> left, space + 1);
    for (int i = 0; i < space; ++i) printf("     ");
    printf("%s\n", root -> word -> buffer);
    if (root -> right) print_tree(root -> right, space + 1);
}

void delete_tree(Node_ptr root)
{
    if (!root) return;
    if (root -> left) delete_tree(root -> left);
    if (root -> right) delete_tree(root -> right);
    free_node(root);
}

void free_node(Node_ptr node)
{
    if (!node) return;
    node -> left = NULL;
    node -> right = NULL;
    node -> parent = NULL;
    delete_string(node -> word);
    free(node);
    node = NULL;
}

// симметричный порядок (от меньшего к большему)
void print_symmetrical_tree(Node_ptr root)
{
    if (!root) return;
    print_symmetrical_tree(root -> left);
    printf("count: %d\n", root -> count);
    printf("word: %s\n\n", root -> word -> buffer);
    print_symmetrical_tree(root -> right);
}

Node_ptr create_root(int count, char * word)
{
    Node_ptr node = (Node_ptr)malloc(sizeof(Node));
    if (!node) return NULL;

    String_ptr string = create_string(word);
    if (!string)
    {
        free(node);
        node = NULL;
        return NULL;
    }

    node -> count = count;
    node -> word = string;

    node -> parent = NULL;
    node -> left = node -> right = NULL;
    return node;
}

Node_ptr add_node(Node_ptr root, int count, char * word)
{
    Node_ptr root_tmp = root;
    Node_ptr parent = NULL;
    Node_ptr tmp = (Node_ptr)malloc(sizeof(Node));
    if (!tmp) return NULL;

    tmp -> count = count;
    tmp -> word = create_string(word);
    if (!(tmp -> word))
    {
        free(tmp);
        tmp = NULL;
    }

    while (root_tmp)
    {
        parent = root_tmp;
        if (strcmp(word, root_tmp -> word -> buffer) < 0) root_tmp = root_tmp -> left;
        else root_tmp = root_tmp -> right;
    }
    tmp -> parent = parent;
    tmp -> left = tmp -> right = NULL;
    if (strcmp(word, parent -> word -> buffer) < 0) parent -> left = tmp;
    else parent -> right = tmp;

    return root;
}

Node_ptr min_node(Node_ptr root)
{
    Node_ptr node = root;
    while (node -> left) node = node -> left;
    return node;
}

Node_ptr max_node(Node_ptr root)
{
    Node_ptr node = root;
    while (node -> right) node = node -> right;
    return node;
}

Node_ptr search_min(Node_ptr root)
{
    Node_ptr root_tmp = root;
    Node_ptr node = NULL;

    if (root -> right) return min_node(root -> right);

    node = root_tmp -> parent;

    while (node && (root_tmp == node -> right))
    {
        root_tmp = node;
        node = node -> parent;
    }
    return node;
}

Node_ptr search_node_via_word(Node_ptr root, char * word)
{
    if (!root || (strcmp(word, root -> word -> buffer) == success)) return root;

    if (strcmp(word, root -> word -> buffer) < 0) return search_node_via_word(root -> left, word);
    else return search_node_via_word(root -> right, word);
}

void delete_node(Node_ptr root, char * word)
{
    Node_ptr root_tmp = root;
    Node_ptr node = NULL;
    Node_ptr parent = NULL;
    node = search_node_via_word(root, word);
    // нет поддеревьев
    if (!(node -> left) && !(node -> right))
    {
        parent = node -> parent;
        if (node == parent -> right) parent -> right = NULL;
        else parent -> left = NULL;
        free_node(node);
    }

    // поддерево справо
    if (!(node -> left) && (node -> right))
    {
        parent = node -> parent;
        if (!parent)
        {
            root = node -> right;
        }
        else
        {
            if (node == parent -> right) parent -> right = node -> right;
            else parent -> left = node -> right;
        }
        free_node(node);
    }

    // поддерево слева
    if ((node -> left) && !(node -> right))
    {
        parent = node -> parent;
        if (!parent)
        {
            root = node -> left;
        }
        else
        {
            if (node == parent -> left) parent -> right = node -> left;
            else parent -> right = node -> left;
        }
        free_node(node);
    }

    // поддеревья и справа, и слева
    if ((node -> left) && (node -> right))
    {
        // найти следующий за удаляемым элементом элемент
        parent = search_min(node);
        node -> word = parent -> word;
        node -> count = parent -> count;
        if (!(parent -> right)) parent -> parent -> left = NULL;
        else parent -> parent -> left = parent -> right;
        free_node(parent);
    }
}