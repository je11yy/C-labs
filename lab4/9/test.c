#include <stdio.h>

int main()
{
    char * string = "123 143\n";
    int first, second;
    sscanf(string, "%d", &first);
    printf("%d\n", first);
    sscanf(string, "%d", &second);
    printf("%d\n", second);
}