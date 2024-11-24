#include "ft_printf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

int main()
{
    ft_printf(" %p %p \n", LONG_MIN, LONG_MAX);
	ft_printf(" %p %p \n", ULONG_MAX, -ULONG_MAX);
	ft_printf(" %10.5p %p \n", 0, 0);

	printf("\n\n");

	printf(" %p %p \n", LONG_MIN, LONG_MAX);
	printf(" %p %p \n", ULONG_MAX, -ULONG_MAX);
	printf(" %10.5p %p \n", 0, 0);
}