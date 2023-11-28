#include "polynomial.h"
#include "list.h"
#include <math.h>

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4,
    incorrect_command_input = -5,
    incorrect_argument = -6
};

// распечатка
void print_polynomial(List_ptr polynomial)
{
    if (!polynomial) return;
    Node_ptr tmp = polynomial -> head;
    if (!tmp) return;
    int i = 0;
    while (tmp)
    {
        if (tmp -> odd > 0 && i != 0) printf("+ ");
        else if (tmp -> odd < 0) printf("- ");
        if (tmp -> power != 0 && tmp -> power != 1)
        {
            if (abs(tmp -> odd) == 1) printf("x^%d ", tmp -> power);
            else printf("%dx^%d ", abs(tmp -> odd), tmp -> power);
        }
        else if (tmp -> power == 1)
        {
            if (abs(tmp -> odd) == 1) printf("x ");
            else printf("%dx ", abs(tmp -> odd));
        }
        else if(tmp -> power == 0) printf("%d ", abs(tmp -> odd));
        i++;
        tmp = tmp -> next;
    }
    printf("\n");
}

// композиция
List_ptr cmps(List_ptr first, List_ptr second)
{
    int power;
    Node_ptr tmp = first -> head;
    int prev_power = 1;
    List_ptr new_list = create_list();
    List_ptr second_copy;
    List_ptr second_copy_2 = copy_list(second);
    List_ptr tmp_list;
    int res;
    while (tmp)
    {
        power = tmp -> power;
        if (power != 0 && power != 1)
        {
            second_copy = copy_list(second_copy_2);
            second = raise_in_power(second, second_copy, power - prev_power);
            prev_power = power;
        }
        tmp_list = create_list();
        res = push(&tmp_list, tmp -> odd, 0); // делаем из текущего коэффициента многочлен
        if (res != success)
        {
            free_list(first);
            free_list(second);
            free_list(new_list);
            return NULL;
        }
        if (power != 0)
        {
            second_copy = copy_list(second);
            tmp_list = mult(tmp_list, second); // умножить коэффициент первого на второй многочлен
            if (!tmp_list)
            {
                free_list(first);
                free_list(new_list);
                return NULL;
            }
            second = copy_list(second_copy);
            free_list(second_copy);
        }
        new_list = add(new_list, tmp_list); // прибавить к предыдущему результату новый
        if (!new_list)
        {
            free_list(first);
            free_list(second);
            return NULL;
        }
        tmp = tmp -> next;
    }
    free_list(first);
    free_list(second);
    free_list(second_copy_2);
    return new_list;
}

// возведение многочлена в степень
List_ptr raise_in_power(List_ptr list, List_ptr prev, int power)
{
    List_ptr copy = copy_list(prev);
    if (!copy) return NULL;
    List_ptr copy_2 = copy_list(prev);
    if (!copy_2)
    {
        free_list(copy);
        return NULL;
    }
    while (power)
    {
        list = mult(list, copy);
        if (power != 1) copy = copy_list(copy_2);
        power--;
    }
    free_list(copy_2);
    free_list(prev);
    return list;
}

// дифференцирование (взять производную)
List_ptr diff(List_ptr list)
{
    List_ptr new_list = create_list();
    if (!new_list)
    {
        free_list(list);
        return NULL;
    }
    Node_ptr tmp = list -> head;
    int result;
    while (tmp)
    {
        if (tmp -> power != 0)
        {
            result = push(&new_list, (tmp -> power) * (tmp -> odd), tmp -> power - 1);
            if (result != success)
            {
                free_list(list);
                free_list(new_list);
                return NULL;
            }
        }
        tmp = tmp -> next;
    }
    free_list(list);
    return new_list;
}

// вычисление многочлена в заданной точке
List_ptr eval(List_ptr list, int value)
{
    int res;
    int result = 0;
    List_ptr new_list = create_list();
    if (!new_list)
    {
        free(list);
        return NULL;
    }
    Node_ptr tmp = list -> head;
    int power;
    int prev_power = 0;
    int pow_value = 1;
    while (tmp)
    {
        power = tmp -> power;
        if (power != 0)
        {
            quick_pow(&pow_value, value, power - prev_power);
            prev_power = power;
        }
        result += (tmp -> odd) * pow_value;
        tmp = tmp -> next;
    }
    res = push(&new_list, result, 0);
    if (res != success)
    {
        free_list(list);
        return NULL;
    }
    return new_list;
}

void quick_pow(int * result, int value, int power)
{
    while (power)
    {
        power--;
        *result *= value;
    }
}

List_ptr mod(int * error, List_ptr first, List_ptr second)
{
    List_ptr copy_first = copy_list(first);
    if (!copy_first)
    {
        free_list(second);
        return NULL;
    }
    List_ptr division_res = division(error, first, second);
    if (*error != success)
    {
        free_list(copy_first);
        return NULL;
    }
    List_ptr res = sub(copy_first, division_res);
    if (!res) return NULL;
    return res;
}

// повышение степени на power
List_ptr increase_power(List_ptr list, int power)
{
    Node_ptr tmp = list -> head;
    List_ptr new_list = create_list();
    if (!new_list)
    {
        free_list(list);
        return NULL;
    }
    int result;
    while (tmp)
    {
        result = push(&new_list, tmp -> odd, tmp -> power + power);
        if (result != success)
        {
            free_list(list);
            free_list(new_list);
            return NULL;
        }
        tmp = tmp -> next;
    }
    free_list(list);
    return new_list;
}

List_ptr division(int * error, List_ptr first, List_ptr second)
{
    if (first -> power < second -> power)
    {
        *error = incorrect_argument;
        free_list(first);
        free_list(second);
        return NULL;
    }
    List_ptr list = create_list(); // хранение результата
    if (!list)
    {
        free_list(first);
        free_list(second);
        *error = no_memory;
        return NULL;
    }
    int result;

    List_ptr remainder = copy_list(first); // тут будут храниться результаты вычета second из first
    if (!remainder)
    {
        free_list(second);
        free_list(list);
        *error = no_memory;
        return NULL;
    }

    int difference;

    List_ptr tmp_second;

    if (!tmp_second)
    {
        free_list(first);
        free_list(remainder);
        free_list(list);
        *error = no_memory;
        return NULL;
    }

    int odd_tmp; // коэффициент, на который нужно домножить second

    List_ptr odd_list; // коэффициент в виде многочлена

    Node_ptr first_last;
    Node_ptr second_last;

    while (second -> power <= remainder -> power)
    {
        difference = remainder -> power - second -> power;
        tmp_second = copy_list(second); // копия second для хранения домноженного second
        if (difference > 0) // нужно домножить second на x в степени difference
        {
            tmp_second = increase_power(tmp_second, difference);
            if (!tmp_second)
            {
                free_list(first);
                free_list(second);
                free_list(remainder);
                free_list(list);
                *error = no_memory;
                return NULL;
            }
        }
        // найдем элементы с наивысшей степенью
        first_last = get_last(remainder -> head);
        second_last = get_last(tmp_second -> head);
        // найдем коэффициент, на который нужно домножить second и домножим
        odd_tmp = (first_last -> odd) / (second_last -> odd);
        odd_list = create_list();
        result = push(&odd_list, odd_tmp, 0);
        if (result != success)
        {
            free_list(first);
            free_list(tmp_second);
            free_list(second);
            free_list(remainder);
            free_list(list);
            *error = result;
            return NULL;
        }
        tmp_second = mult(odd_list, tmp_second);
        // теперь в tmp_second хранится то, что нужно вычесть из first
        // odd_list - удалено

        // теперь нужно вычесть из remainder получившийся результат, записать в remainder, а также добавить в результат то, на что домножаем

        remainder = sub(remainder, tmp_second);
        //tmp_second - удалено
        if (odd_tmp != 0)
        {
            result = push(&list, odd_tmp, difference); // добавление в результат
            if (result != success)
            {
                free_list(first);
                free_list(tmp_second);
                free_list(second);
                free_list(remainder);
                *error = no_memory;
                return NULL;
            }
        }
    }
    *error = success;
    return list;
}

List_ptr mult(List_ptr first, List_ptr second)
{
    if (first -> size == 1 && first -> head -> odd == 0) // значение сумматора - 0
    {
        free_list(second);
        return first;
    }

    Node_ptr first_head = first -> head;
    Node_ptr second_head = second -> head;
    Node_ptr node;
    int power;
    List_ptr tmp_list;

    List_ptr new_list = create_list();
    int result;
    int odd;
    result = push(&new_list, 0, 0);
    if (result != success)
    {
        free_list(first);
        free_list(second);
        free_list(new_list);
        return NULL;
    }
    while (first_head)
    {
        second_head = second -> head;
        while (second_head)
        {
            tmp_list = create_list();
            odd = (first_head -> odd) * (second_head -> odd);
            if (odd == 0)
            {
                second_head = second_head -> next;
                continue;
            }
            power = first_head -> power + second_head -> power;
            result = push(&tmp_list, odd, power);
            if (result != success)
            {
                free_list(first);
                free_list(second);
                free_list(new_list);
                return NULL;
            }
            new_list = add(new_list, tmp_list);
            if (!new_list)
            {
                free_list(first);
                free_list(second);
                return NULL;
            }
            second_head = second_head -> next;
        }
        first_head = first_head -> next;
    }
    free_list(first);
    free_list(second);
    return new_list;
}

List_ptr make_negative(List_ptr list)
{
    Node_ptr tmp = list -> head;
    List_ptr new_list = create_list();
    if (!new_list)
    {
        free_list(list);
        return NULL;
    }
    int result;
    while (tmp)
    {
        result = push(&new_list, -(tmp -> odd), tmp -> power);
        if (result != success)
        {
            free_list(list);
            free_list(new_list);
            return NULL;
        }
        tmp = tmp -> next;
    }
    free_list(list);
    return new_list;
}

List_ptr sub(List_ptr first, List_ptr second)
{
    List_ptr negative_second = make_negative(second);
    if (!negative_second)
    {
        free_list(first);
        return NULL;
    }
    List_ptr new_list = add(first, negative_second);
    if (!new_list)
    {
        free_list(first);
        free_list(negative_second);
        return NULL;
    }
    return new_list;
}

List_ptr add(List_ptr first, List_ptr second)
{
    if (first -> size == 1 && first -> head -> odd == 0) // значение сумматора - 0
    {
        free_list(first);
        return second;
    }
    Node_ptr first_head = first -> head;
    Node_ptr second_head = second -> head;
    int power;
    Node_ptr node;

    List_ptr new_list = create_list();
    int result;

    int sum = 0;

    while (first_head)
    {
        power = first_head -> power;
        node = find_power(second_head, power);
        if (!node)
        {
            result = push(&new_list, first_head -> odd, power);
            if (result != success)
            {
                free_list(first);
                free_list(second);
                free_list(new_list);
                return NULL;
            }
        }
        else
        {
            sum = first_head -> odd + node -> odd;
            if (sum != 0)
            {
                result = push(&new_list, first_head -> odd + node -> odd, power);
                if (result != success)
                {
                    free_list(first);
                    free_list(second);
                    free_list(new_list);
                    return NULL;
                }
            }
        }
        first_head = first_head -> next;
    }
    first_head = first -> head;
    while (second_head)
    {
        power = second_head -> power;
        node = find_power(first_head, power);
        if (!node)
        {
            result = push(&new_list, second_head -> odd, power);
            if (result != success)
            {
                free_list(first);
                free_list(second);
                free_list(new_list);
                return NULL;
            }
        }
        second_head = second_head -> next;
    }

    free_list(first);
    free_list(second);
    return new_list;
}

Node_ptr find_power(Node_ptr head, int power)
{
    Node_ptr tmp = head;
    while (tmp)
    {
        if (tmp -> power == power)
        {
            return tmp;
        }
        tmp = tmp -> next;
    }
    return NULL; // не найден элемент с такой степенью
}