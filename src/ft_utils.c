/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivar <ivar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 08:43:43 by ivar              #+#    #+#             */
/*   Updated: 2025/01/19 11:59:29 by ivar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

void	ft_format_diux(t_format *format)
{
	if (format->specifier == 'd' || format->specifier == 'i'
		|| format->specifier == 'u')
		format->alternate_form = 0;
	if (format->specifier == 'u' || format->specifier == 'x'
		|| format->specifier == 'X')
	{
		format->plus = 0;
		format->space = 0;
	}
}

void	ft_reverse_substring(char *str, int start, int end)
{
	char	temp;

	while (start < end)
	{
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

static int	apply_sign(char **buf, t_format *fmt, long n, int *len)
{
	if (n < 0)
	{
		*(*buf)++ = '-';
		*len = (*len) + 1;
	}
	else if (fmt->plus)
	{
		*(*buf)++ = '+';
		*len = (*len) + 1;
	}
	else if (fmt->space)
	{
		*(*buf)++ = ' ';
		*len = (*len) + 1;
	}
	else if (fmt->precision == -1)
	{
		*(*buf)++ = fmt->zero_padding;
	}
	return (*len);
}

static void	handle_alternate(char **buf, t_format *fmt, long n, int *len)
{
	if (fmt->alternate_form == 1 && n != 0)
	{
		(*len) += 1;
		if (fmt->zero_padding == ' ')
		{
			(*len) += 1;
			*(*buf)++ = fmt->specifier;
			*(*buf)++ = '0';
		}
	}
}

int	apply_padding_and_prefix(char **buf, t_format *fmt, long n, int *len)
{
	int	end;

	while (fmt->precision != -1 && *len < fmt->precision)
	{
		(*len)++;
		*(*buf)++ = '0';
	}
	handle_alternate(buf, fmt, n, len);
	if (fmt->zero_padding == '0' && (*len) < fmt->width - 1)
		while ((*len)++ < fmt->width - 1)
			*(*buf)++ = fmt->zero_padding;
	if (fmt->alternate_form == 1 && n != 0 && fmt->zero_padding == '0')
	{
		*(*buf)++ = fmt->specifier;
		*(*buf)++ = '0';
	}
	end = apply_sign(buf, fmt, n, len);
	if (fmt->zero_padding == ' ' && (*len) < fmt->width)
		while ((*len)++ < fmt->width)
			*(*buf)++ = fmt->zero_padding;
	return (end);
}
