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

void print_error(int error);

Node_ptr make_tree(FILE * file, char separators[], int size);
int in_separators(char separators[], int size, char symbol);
int switch_command(Node_ptr * root, int command);
Node_ptr search_node_via_length(Node_ptr root, int length);
void print_most_quantity(Node_ptr root);
void print_nodes(Node_ptr root, int * quantity, int length);
void get_minimum_length(Node_ptr root, int * length);
void get_maximum_length(Node_ptr root, int * length);
int get_count(Node_ptr root);
int get_command();
void get_maximum_count(Node_ptr root, int * count);
void menu();
void input_in_file(FILE * file, Node_ptr root);
Node_ptr recreate_tree(Node_ptr root);


int main(int argc, char * argv[])
{
    if (argc < 3)
    {
        print_error(incorrect_input);
        return incorrect_input;
    }
    FILE * file = fopen(argv[1], "r");
    if (!file)
    {
        print_error(incorrect_input);
        return incorrect_input;
    }
    char separators[argc - 2];
    for (int i = 2, j = 0; i < argc; ++i, ++j)
    {
        if (strlen(argv[i]) != 1)
        {
            fclose(file);
            print_error(incorrect_input);
            return incorrect_input;
        }
        separators[j] = argv[i][0];
    }

    Node_ptr root = make_tree(file, separators, argc - 2);
    if (!root)
    {
        print_error(no_memory);
        fclose(file);
        return no_memory;
    }
    fclose(file);
    
    int command = -1;
    int result;
    while (command != 0)
    {
        menu();
        printf("Input form: ");
        command = get_command();
        while (command < 0 || command > 8)
        {
            printf("Try again: ");
            command = get_command();
        }
        if (command == 0) break;
        result = switch_command(&root, command);
        if (result != success)
        {
            print_error(result);
            delete_tree(root);
            return result;
        }
    }

    delete_tree(root);
    printf("Program has finished correctly.\n");
    return success;
}

Node_ptr recreate_tree(Node_ptr root)
{
    printf("\nInput name of the file (less than 50 characters): ");
    int length = 51;
    char file_name[length];
    int result = scanf("%s", file_name);
    FILE * file;
    while (result != 1 || !(file = fopen(file_name, "w")))
    {
        printf("\nTry again: ");
        fflush(stdin);
        result = scanf("%s", file_name);
    }
    printf("\n\nPrevious tree:\n\n");
    print_tree(root, 0);
    input_in_file(file, root);
    delete_tree(root);
    fclose(file);
    file = fopen(file_name, "r");
    if (!file) return NULL;
    root = make_tree(file, " ", 1);
    if (!root)
    {
        fclose(file);
        return NULL;
    }
    printf("\n\nRecreated tree:\n\n");
    print_tree(root, 0);
    printf("\n\n");
    fclose(file);
    return root;
}

void input_in_file(FILE * file, Node_ptr root)
{
    if (!root) return;
    for (int i = 0; i < root -> count; ++i)
    {
        fprintf(file, "%s ", root -> word -> buffer);
    }
    input_in_file(file, root -> left);
    input_in_file(file, root -> right);
}

// максимальная глубина - это высота дерева
int get_max_depth(Node_ptr root)
{
    if (!root) return 0;
    int left, right;
    if (root -> left) left = get_max_depth(root -> left);
    else left = -1;
    if (root -> right) right = get_max_depth(root -> right);
    else right = -1;
    int max = left > right ? left : right;
    return max;
}

int switch_command(Node_ptr * root, int command)
{
    int result;
    Node_ptr node;
    switch (command)
    {
        case 0: break;
        case 1:
            result = get_count(*root);
            printf("The quantity of the word in file: %d\n\n", result);
            break;
        case 2:
            print_most_quantity(*root);
            break;
        case 3:
            result = 0;
            get_maximum_length(*root, &result);
            node = search_node_via_length(*root, result);
            if (!node) break;
            printf("\nThe word with maximum length: %s - %d\n\n", node -> word -> buffer, result);
            break;
        case 4:
            result = 0;
            get_minimum_length(*root, &result);
            node = search_node_via_length(*root, result);
            if (!node) break;
            printf("\nThe word with minimum length: %s - %d\n\n", node -> word -> buffer, result);
            break;
        case 5:
            print_tree(*root, 0);
            break;
        case 6:
            print_symmetrical_tree(*root);
            break;
        case 7:
            result = get_max_depth(*root);
            printf("\nMax depth: %d\n\n", result);
            break;
        case 8:
            *root = recreate_tree(*root);
            if (!root) return no_memory;
            break;
    }
    return success;
}

Node_ptr search_node_via_length(Node_ptr root, int length)
{
    if (!root || (root -> word -> length == length)) return root;
    if (root -> left) return search_node_via_length(root -> left, length);
    if (root -> right) return search_node_via_length(root -> right, length);
}

void print_most_quantity(Node_ptr root)
{
    int n;
    printf("\nInput N: ");
    int result = scanf("%d", &n);
    while (result != 1)
    {
        fflush(stdin);
        printf("\nTry again: ");
        result = scanf("%d", &n);
        if (n <= 0) result = 0;
    }
    int count = 0;
    get_maximum_count(root, &count);
    printf("N (or less if not so many words) words with most quantity in file:\n");
    while (n != 0 && count != 0)
    {
        print_nodes(root, &n, count);
        count--;
    }
    printf("\n");
}

void get_maximum_count(Node_ptr root, int * count)
{
    if (!root) return;
    if (root -> left) get_maximum_count(root -> left, count);

    if (root -> count > *count) *count = root -> count;

    if (root -> right) get_maximum_count(root -> right, count);
}

void print_nodes(Node_ptr root, int * quantity, int length)
{
    if (!root) return;
    if (root -> right) print_nodes(root -> right, quantity, length);

    if (root -> count == length && (*quantity))
    {
        (*quantity)--;
        printf("%s - %d\n", root -> word -> buffer, root -> count);
    }

    if (root -> left) print_nodes(root -> left, quantity, length);
}

void get_minimum_length(Node_ptr root, int * length)
{
    if (!root) return;
    if (root -> left) get_minimum_length(root -> left, length);

    int len = root -> word -> length;
    if (*length == 0) *length = len;
    else if (len < *length) *length = len;

    if (root -> right) get_minimum_length(root -> right, length);
}

void get_maximum_length(Node_ptr root, int * length)
{
    if (!root) return;
    if (root -> left) get_maximum_length(root -> left, length);

    int len = root -> word -> length;
    if (len > *length) *length = len;

    if (root -> right) get_maximum_length(root -> right, length);
}

int get_count(Node_ptr root)
{
    printf("\n\nInput word: ");
    int length = 0;
    get_maximum_length(root, &length);
    char word[length];
    int result = scanf("%s", word);
    while (result != 1)
    {
        printf("\nTry again: ");
        fflush(stdin);
        result = scanf("%s", word);
    }
    Node_ptr node = search_node_via_word(root, word);
    if (!node) return 0;
    return node -> count;
}

int get_command()
{
    int command;
    int result = scanf("%d", &command);
    while (result != 1)
    {
        fflush(stdin);
        printf("\nTry again: ");
        result = scanf("%d", &command);
    }
    return command;
}

void menu()
{
    printf("1. Count of the word in file.\n");
    printf("2. First N words with most quantity.\n");
    printf("3. Maximum length word.\n");
    printf("4. Minimum length word.\n");
    printf("5. Print tree.\n");
    printf("6. Print tree-information.\n");
    printf("7. Print max depth of the tree (its height).\n");
    printf("8. Input tree in file and recreate it.\n");
    printf("0. Exit.\n");
}

Node_ptr make_tree(FILE * file, char separators[], int size)
{
    Node_ptr root = NULL;
    Node_ptr node = NULL;
    int count;
    int all_words_quantity = 0;

    char * temp = NULL;
    char * tmp = NULL;
    int length = 0;

    char symbol = '0';
    int result;
    while (symbol != EOF)
    {
        symbol = fgetc(file);
        if (symbol == EOF) break;
        if ((in_separators(separators, size, symbol) == success || symbol == '\n' || symbol == '\t') && length != 0 )
        {
            length++;
            tmp = (char*)realloc(temp, length * sizeof(char));
            if (!tmp)
            {
                free(temp);
                temp = NULL;
                delete_tree(root);
                return NULL;
            }
            temp = tmp;
            temp[length - 1] = '\0';
            if (all_words_quantity == 0)
            {
                root = create_root(1, temp);
                if (!root)
                {
                    free(temp);
                    temp = NULL;
                    return NULL;
                }
            }
            else
            {
                node = search_node_via_word(root, temp);
                if (node) node -> count = node -> count + 1;
                else
                {
                    count = 1;
                    node = add_node(root, count, temp);
                    if (!node)
                    {
                        delete_tree(root);
                        free(temp);
                        temp = NULL;
                        return NULL;
                    }
                    root = node;
                }
            }
            free(temp);
            temp = NULL;
            length = 0;
            all_words_quantity++;
        }
        else if (!(in_separators(separators, size, symbol) == success || symbol == '\n' || symbol == '\t'))
        {
            length++;
            tmp = (char*)realloc(temp, length * sizeof(char));
            if (!tmp)
            {
                free(temp);
                temp = NULL;
                delete_tree(root);
                return NULL;
            }
            temp = tmp;
            temp[length - 1] = symbol;
        }
    }
    if (length != 0)
    {
        length++;
        tmp = (char*)realloc(temp, length * sizeof(char));
        if (!tmp)
        {
            free(temp);
            temp = NULL;
            delete_tree(root);
            return NULL;
        }
        temp = tmp;
        temp[length - 1] = '\0';
        if (all_words_quantity == 0)
        {
            root = create_root(1, temp);
            if (!root)
            {
                free(temp);
                temp = NULL;
                return NULL;
            }
        }
        else
        {
            node = search_node_via_word(root, temp);
            if (node) node -> count = node -> count + 1;
            else
            {
                count = 1;
                node = add_node(root, count, temp);
                if (!node)
                {
                    delete_tree(root);
                    free(temp);
                    temp = NULL;
                    return NULL;
                }
                root = node;
            }
        }
        free(temp);
        temp = NULL;
        length = 0;
        all_words_quantity++;
    }
    return root;
}

int in_separators(char separators[], int size, char symbol)
{
    for (int i = 0; i < size; ++i)
    {
        if (separators[i] == symbol) return success;
    }
    return fail;
}

void print_error(int error)
{
    switch(error)
    {
        case no_memory:
            printf("\nComputer doesnot have enough memory for this calculation.");
            break;
        case incorrect_input:
            printf("\nIncorrect input.\n");
            break;
        case file_open_error:
            printf("\nCannot open file!\n");
            break;
    }
}