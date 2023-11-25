#include "tree.h"
#include "arithmetic.h"
#include "stack.h"
#include <string.h>
#include <ctype.h>
#include <time.h>

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
    delete_file_error = -8,
    incorrect_file_input = -9,
    incorrect_operator = -10
};

int make_truth_table(char * file_name);
int get_variables(char ** variables, char * infix);
char * generate_file_name();
int calculate_tree(Node_ptr root, const char * variables, const int * values, const int var_number);
int infix_to_postfix(int * error, char ** postfix_result, const char * infix);
int make_tree(Node_ptr * tree_result, int * error, char * infix);
void generate_truth_table(Node_ptr root, const int num_variables, const char * variables);
int is_bracket(char symbol);
void print_variables(FILE * file, const char * variables, int count);
void print_error(int error);
int not_in(char * vars, int count, char symbol);

int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        print_error(incorrect_file_input);
        return incorrect_file_input;
    }
    int result = make_truth_table(argv[1]);
    if (result != success)
    {
        print_error(result);
        return result;
    }
    printf("Program has finished correcly.\n");
    return success;
}

int make_truth_table(char * file_name)
{
    FILE * input = fopen(file_name, "r");
    if (!input) return file_open_error;
    char * line = NULL;
    size_t len = 0;
    getline(&line, &len, input);
    int length = strlen(line);
    if (!length) // пустой файл
    {
        free(line);
        line = NULL;
        fclose(input);
        return incorrect_file_input;
    }
    if (!feof(input)) // в файле еще есть строки
    {
        free(line);
        line = NULL;
        fclose(input);
        return incorrect_file_input;
    }
    fclose(input);
    int error;
    Node_ptr root = NULL;
    int result = make_tree(&root, &error, line);
    if (result != success)
    {
        free(line);
        line = NULL;
        return error;
    }
    char * vars = NULL;
    int var_count = get_variables(&vars, line);
    if (var_count < 0)
    {
        free_tree(root);
        return no_memory;
    }
    free(line);
    line = NULL;
    generate_truth_table(root, var_count, vars);
    free(vars);
    vars = NULL;
    free_tree(root);
    return success;
}

int not_in(char * vars, int count, char symbol)
{
    for (int i = 0; i < count; ++i)
    {
        if (vars[i] == symbol) return fail;
    }
    return success;
}

int get_variables(char ** variables, char * infix)
{
    int length = 0;
    char * temp;
    int len = strlen(infix);
    for (int i = 0; i < len; ++i)
    {
        if (isalpha(infix[i]) && not_in(*variables, length, infix[i]) == success)
        {
            length++;
            temp = (char*)realloc(*variables, length * sizeof(char));
            if (!temp)
            {
                if (*variables)
                {
                    free(*variables);
                    *variables = NULL;
                }
                free(infix);
                infix = NULL;
                return no_memory;
            }
            *variables = temp;
            (*variables)[length - 1] = infix[i];
        }
    }
    return length;
}

char * generate_file_name()
{
    srand(time(NULL));
    int max_length = 10, min_length = 1;

    int length = rand() % (max_length - min_length + 1) + min_length; // длина названия

    char * file_name = (char*)malloc((length + 6) * sizeof(char));
    file_name[length + 6] = 0;
    for (int i = 0; i <= length;)
    {
        file_name[i] = rand() % ('Z' - '0' + 1) + '0';
        if (isalpha(file_name[i]) || isdigit(file_name[i])) i++;
    }
    file_name[length + 1] = '.';
    file_name[length + 2] = 't';
    file_name[length + 3] = 'x';
    file_name[length + 4] = 't';
    printf("Generated file name: %s\n", file_name);
    return file_name;
}

int calculate_tree(Node_ptr root, const char * variables, const int * values, const int var_number)
{
    if (!root) return 0;
    int left = calculate_tree(root -> left, variables, values, var_number);
    int right = calculate_tree(root -> right, variables, values, var_number);
    if (isalpha(root -> data))
    {
        for (int i = 0; i < var_number; ++i)
        {
            if (root -> data == variables[i]) return values[i];
        }
    }
    if (root -> data == '0' || root -> data == '1') return root -> data - '0';

    // если оператор
    switch (root -> data)
    {
        case '&': return left & right;
        case '|': return left | right;
        case '~': return ~left;
        case '-': return left <= right;
        case '+': return ~(~left | right);
        case '<': return left != right;
        case '=': return left == right;
        case '!': return ~(left & right);
        case '?': return ~(left | right);
        default: return fail;
    }
}

int infix_to_postfix(int * error, char ** postfix_result, const char * infix)
{
    Char_stack_ptr stack = NULL;
    int count = 0;
    char * temp;

    char * postfix = (char*)calloc((2 * strlen(infix) + 1), sizeof(char));
    if (!postfix)
    {
        *error = runtime_error;
        return no_memory;
    }
    int index = 0;
    Char_stack_ptr node;


    int res;
    int i = 0;
    int flag = 1;
    char symbol;
    while (infix[i] != '\n' && infix[i] != '\0')
    {
        symbol = infix[i];
        if ((isalpha(symbol) && isupper(symbol)) || symbol == '0' || symbol == '1')
        {
            postfix[index++] = symbol;
            flag = 0;
        }
        else if (is_operator(symbol) == success)
        {   
            if ((symbol == '+' || symbol == '-' || symbol == '<') && infix[i + 1] != '>')
            {
                *error = incorrect_operator;
                free_stack_char(stack);
                free(postfix);
                postfix = NULL;
                return runtime_error;
            }
            if (i != 0)
            {
                if (!(infix[i - 1] == '+' || infix[i - 1] == '-' || infix[i - 1] == '<') && symbol == '>')
                {
                    *error = incorrect_operator;
                    free_stack_char(stack);
                    free(postfix);
                    postfix = NULL;
                    return runtime_error;
                }
            }
            if (symbol == '>')
            {
                i++;
                continue;
            }
            if (flag && symbol != '~')
            {
                free_stack_char(stack);
                free(postfix);
                postfix = NULL;
                *error = incorrect_input;
                return runtime_error;
            }
            else flag = 1;
            node = get_last_char(stack);
            while (stack && priority(node -> data) >= priority(symbol))
            {
                postfix[index++] = node -> data;
                pop_back_char(&stack);
                node = get_last_char(stack);
            }
            res = push_back_char(&stack, symbol);
            node = get_last_char(stack);
            if (res != success)
            {
                free_stack_char(stack);
                free(postfix);
                postfix = NULL;
                return no_memory;
            }
        }
        else if (is_bracket(symbol) == success)
        {
            if (symbol == '(')
            {
                flag = 1;
                res = push_back_char(&stack, symbol);
                node = get_last_char(stack);
                if (res != success)
                {
                    free_stack_char(stack);
                    free(postfix);
                    postfix = NULL;
                    return no_memory;
                }
            }
            else if (symbol == ')')
            {
                node = get_last_char(stack);
                while (stack && node -> data != '(')
                {
                    postfix[index++] = node -> data;
                    pop_back_char(&stack);
                    node = get_last_char(stack);
                }
                if (!stack)
                {
                    free_stack_char(stack);
                    free(postfix);
                    postfix = NULL;
                    *error = invalid_brackets;
                    return runtime_error;
                }
                pop_back_char(&stack);
            }
        }
        else if (symbol != ' ' && symbol != '\n' && symbol != '\t')
        {
            free_stack_char(stack);
            free(postfix);
            postfix = NULL;
            *error = invalid_arguments;
            return runtime_error;
        }
        i++;
    }

    while (stack)
    {
        if (stack -> data == '(')
        {
            free_stack_char(stack);
            free(postfix);
            postfix = NULL;
            *error = invalid_brackets;
            return runtime_error;
        }
        node = get_last_char(stack);
        postfix[index++] = node -> data;
        pop_back_char(&stack);
    }
    postfix[index] = 0;
    if (postfix[index - 1] == 0)
    {
        *error = incorrect_input;
        free_stack_char(stack);
        free(postfix);
        postfix = NULL;
        return runtime_error;
    }
    free_stack_char(stack);
    *postfix_result = postfix;
    return success;
}

int is_bracket(char symbol)
{
    if (symbol == '(' || symbol == ')') return success;
    return fail;
}

int make_tree(Node_ptr * tree_result, int * error, char * infix)
{
    Node_ptr root = NULL;
    char * postfix = NULL;
    int result = infix_to_postfix(error, &postfix, infix);
    if (result != success) return runtime_error;
    int length = strlen(postfix);
    Node_ptr node;
    Tree_stack_ptr stack = NULL;
    char symbol;

    Tree_stack_ptr first_var;
    Tree_stack_ptr second_var;
    for (int i = 0; i < length; ++i)
    {
        symbol = postfix[i];
        if ((isalpha(symbol) && isupper(symbol)) || symbol == '0' || symbol == '1')
        {
            node = create_tree_node(symbol, NULL, NULL);
            if (!node)
            {
                free_tree(root);
                free(postfix);
                free_stack_tree(stack);
                postfix = NULL;
                *error = no_memory;
                return runtime_error;
            }
            result = push_back_tree(&stack, node);
            if (result != success)
            {
                *error = result;
                free_tree(root);
                free_stack_tree(stack);
                free(postfix);
                postfix = NULL;
                return runtime_error;
            }
        }
        else if (is_operator(symbol) == success)
        {
            first_var = get_last_tree(stack);
            if (symbol == '~')
            {
                node = create_tree_node(symbol, first_var -> node, NULL);
            }
            else
            {
                second_var = get_prev_last_tree(stack);
                node = create_tree_node(symbol, first_var -> node, second_var -> node);
            }
            if (!node)
            {
                free_tree(root);
                free(postfix);
                free_stack_tree(stack);
                postfix = NULL;
                *error = no_memory;
                return runtime_error;
            }
            if (symbol == '~') pop_back_tree(&stack);
            else
            {
                pop_back_tree(&stack);
                pop_back_tree(&stack);
            }
            result = push_back_tree(&stack, node);
            if (result != success)
            {
                *error = result;
                free_stack_tree(stack);
                return runtime_error;
            }
        }
    }
    free_stack_tree(stack);
    free(postfix);
    postfix = NULL;
    *tree_result = node;
    return success;
}

void generate_truth_table(Node_ptr root, const int num_variables, const char * variables)
{
    char * file_name = generate_file_name();
    FILE * file = fopen(file_name, "w");
    Node_ptr tmp = root;
    // сначала распечатаем все переменные
    print_variables(file, variables, num_variables);
    // теперь распечатаем выражение
    fprintf(file, " ");
    print_infix(file, tmp);
    fprintf(file, "\n");
    // теперь сделаем перебор значений переменных и результатов вычислений выражения для них
    int combinations = 1 << num_variables;
    int variables_assignment[num_variables];
    int result;
    for (int i = 0; i < combinations; ++i)
    {
        for (int j = 0; j < num_variables; ++j)
        {
            fprintf(file, " %d |", (i >> j) & 1);
            variables_assignment[j] = (i >> j) & 1;
        }
        // печатаем результат вычисления выражения с этими значениями
        result = calculate_tree(root, variables, variables_assignment, num_variables);
        fprintf(file, " %d\n", result);
    }
    fclose(file);
    free(file_name);
    file_name = NULL;
}

void print_variables(FILE * file, const char * variables, int count)
{
    for (int i = 0; i < count; ++i)
    {
        fprintf(file, " %c |", variables[i]);
    }
}

void print_error(int error)
{
    switch (error)
    {
        case incorrect_input:
            printf("Invalid expression.\n");
            break;
        case invalid_brackets:
            printf("Parentheses mismatch.\n");
            break;
        case invalid_arguments:
            printf("Invalid arguments in expression.\n");
            break;
        case incorrect_file_input:
            printf("Incorrect input.\n");
            break;
        case no_memory:
            printf("Computer doesn't have enough memory.\n");
            break;
        case incorrect_operator:
            printf("Incorrect operator in expression.\n");
            break;
        default: break;
    }
}