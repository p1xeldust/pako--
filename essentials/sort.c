/**
 * Written by Paul Goldstein, December 2023.
 */

#include <stdio.h>
#include <string.h>

// Когда-нибудь понадобится

int sort_array(char **a, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
            if (strcmp(a[i], a[j]) > 0)
            {
                char *temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        printf("%s ", a[i]);
    }
    return 0;
}