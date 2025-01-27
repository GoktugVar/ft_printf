/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivar <ivar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 08:43:31 by ivar              #+#    #+#             */
/*   Updated: 2025/01/19 11:42:08 by ivar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

void	init_format(t_format *format)
{
	format->alternate_form = 0;
	format->left_align = -1;
	format->plus = 0;
	format->precision = -1;
	format->space = 0;
	format->specifier = 0;
	format->width = 0;
	format->zero_padding = ' ';
}

void	parse_flags(const char **str, t_format *format)
{
	while (**str == '#' || **str == '-' || **str == '+' || **str == ' '
		|| **str == '0')
	{
		if (**str == '#')
			format->alternate_form = 1;
		else if (**str == '-')
			format->left_align = 1;
		else if (**str == '+')
			format->plus = 1;
		else if (**str == ' ')
			format->space = 1;
		else if (**str == '0')
			format->zero_padding = '0';
		(*str)++;
	}
}

int	parse_width(const char **str, t_format *format, va_list args)
{
	if (**str == '*')
	{
		format->width = va_arg(args, int);
		(*str)++;
	}
	else
	{
		while (**str >= '0' && **str <= '9')
		{
			if (format->width > (2147483647 - (**str - '0')) / 10)
				return (-1);
			format->width = format->width * 10 + (*((*str)++) - '0');
		}
	}
	return (1);
}

int	parse_precision(const char **str, t_format *format, va_list args)
{
	if (**str == '.')
	{
		(*str)++;
		if (**str == '*')
		{
			format->precision = va_arg(args, int);
			(*str)++;
		}
		else
		{
			format->precision = 0;
			while (**str >= '0' && **str <= '9')
			{
				if (format->precision > (2147483647 - (**str - '0') / 10))
					return (-1);
				format->precision *= 10;
				format->precision += (*((*str)++) - '0');
			}
		}
	}
	else
		format->precision = -1;
	return (1);
}

int	parse_specifier(const char **str, t_format *format)
{
	if (**str == 'c' || **str == 's' || **str == 'p' || **str == 'd'
		|| **str == '%' || **str == 'i' || **str == 'u' || **str == 'x'
		|| **str == 'X')
	{
		format->specifier = *((*str)++);
		return (1);
	}
	return (-1);
}
