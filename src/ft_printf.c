/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivar <ivar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 08:43:37 by ivar              #+#    #+#             */
/*   Updated: 2025/01/19 11:26:21 by ivar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"
#include <stdlib.h>
#include <unistd.h>

static int	ft_lenght_arg(t_format *format, va_list args)
{
	if (format->specifier == 'c')
		return (ft_length_c(format, va_arg(args, int)));
	else if (format->specifier == 's')
		return (ft_length_s(format, va_arg(args, char *)));
	else if (format->specifier == 'p')
		return (ft_length_p(format, va_arg(args, void *)));
	else if (format->specifier == 'd' || format->specifier == 'i')
		return (ft_length_n(format, va_arg(args, int), 10));
	else if (format->specifier == 'u')
		return (ft_length_n(format, va_arg(args, unsigned int), 10));
	else if (format->specifier == 'x')
		return (ft_length_n(format, va_arg(args, unsigned int), 16));
	else if (format->specifier == 'X')
		return (ft_length_n(format, va_arg(args, unsigned int), 16));
	else if (format->specifier == '%')
		return (1);
	return (-1);
}

static int	get_format_len(const char *str, va_list args)
{
	int			checker;
	int			len;
	t_format	format;

	len = 0;
	while (*str)
	{
		if (*(str++) == '%')
		{
			init_format(&format);
			parse_flags(&str, &format);
			if (parse_width(&str, &format, args) == -1
				|| parse_precision(&str, &format, args) == -1
				|| parse_specifier(&str, &format) == -1)
				return (-1);
			checker = ft_lenght_arg(&format, args);
			if (checker == -1)
				return (-1);
			len += checker;
		}
		else
			len++;
	}
	return (len);
}

static void	arg_to_buffer(char **buffer, t_format *format, va_list args)
{
	if (format->specifier == 'c')
		ft_c_to_buffer(buffer, format, va_arg(args, int));
	else if (format->specifier == 's')
		ft_s_to_buffer(buffer, format, va_arg(args, char *));
	else if (format->specifier == 'p')
		ft_p_to_buffer(buffer, format, va_arg(args, void *));
	else if (format->specifier == 'd' || format->specifier == 'i')
		ft_n_to_buffer(buffer, format, va_arg(args, int), "0123456789");
	else if (format->specifier == 'u')
		ft_n_to_buffer(buffer, format, va_arg(args, unsigned int),
			"0123456789");
	else if (format->specifier == 'x')
		ft_n_to_buffer(buffer, format, va_arg(args, unsigned int),
			"0123456789abcdef");
	else if (format->specifier == 'X')
		ft_n_to_buffer(buffer, format, va_arg(args, unsigned int),
			"0123456789ABCDEF");
	else if (format->specifier == '%')
		*(*buffer)++ = '%';
}

static void	set_buffer(const char *str, char *buffer, va_list args)
{
	t_format	format;

	while (*str)
	{
		if (*str == '%')
		{
			str++;
			init_format(&format);
			parse_flags(&str, &format);
			parse_width(&str, &format, args);
			parse_precision(&str, &format, args);
			parse_specifier(&str, &format);
			arg_to_buffer(&buffer, &format, args);
		}
		else
			*(buffer++) = *(str++);
	}
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		len;
	char	*buffer;

	if (!str)
		return (-1);
	va_start(args, str);
	len = get_format_len(str, args);
	va_end(args);
	if (len == -1)
		return (-1);
	buffer = (char *)malloc(len + 1);
	if (!buffer)
		return (-1);
	va_start(args, str);
	set_buffer(str, buffer, args);
	if (write(1, buffer, len) == -1)
		return (va_end(args), free(buffer), -1);
	return (va_end(args), free(buffer), len);
}
