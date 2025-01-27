/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buffer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivar <ivar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 08:43:21 by ivar              #+#    #+#             */
/*   Updated: 2025/01/19 11:58:42 by ivar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

void	ft_c_to_buffer(char **buffer, t_format *format, int c)
{
	if (format->left_align == -1)
		while (format->width-- > 1)
			*(*buffer)++ = ' ';
	*(*buffer)++ = (char)c;
	if (format->left_align == 1)
		while (format->width-- > 1)
			*(*buffer)++ = ' ';
}

void	ft_s_to_buffer(char **buffer, t_format *format, char *str)
{
	int	str_len;

	if (!str && format->precision < 6 && format->precision != -1)
		str = "";
	else if (!str)
		str = "(null)";
	str_len = ft_strlen(str);
	if (format->precision != -1 && format->precision < str_len)
		str_len = format->precision;
	if (format->left_align == -1 && format->width > str_len)
		while (0 < ((format->width--) - str_len))
			*(*buffer)++ = ' ';
	else
		format->width -= (str_len - 1);
	while (str_len-- > 0)
		*(*buffer)++ = *(str++);
	if (format->left_align == 1 && format->width > 0)
		while (1 < format->width--)
			*(*buffer)++ = ' ';
}

static int	ft_num_base_to_buffer(char **buf, unsigned long n, char *base)
{
	int				len;
	unsigned long	base_len;

	base_len = ft_strlen(base);
	*(*buf)++ = base[n % base_len];
	len = 1;
	if (n >= base_len)
		len += ft_num_base_to_buffer(buf, n / base_len, base);
	return (len);
}

void	ft_n_to_buffer(char **buf, t_format *fmt, long n, char *base)
{
	int		len;
	char	*start;
	int		end;

	start = *buf;
	ft_format_diux(fmt);
	if (fmt->precision != -1 || fmt->left_align != -1)
		fmt->zero_padding = ' ';
	if (n < 0)
		len = ft_num_base_to_buffer(buf, -n, base);
	else
		len = ft_num_base_to_buffer(buf, n, base);
	if (n == 0 && fmt->precision == 0)
	{
		*--(*buf) = '\0';
		len = 0;
	}
	end = apply_padding_and_prefix(buf, fmt, n, &len);
	if (fmt->left_align != -1)
		ft_reverse_substring(start, 0, end - 1);
	else
		ft_reverse_substring(start, 0,
			ft_length_n(fmt, n, ft_strlen(base)) - 1);
	*buf = start + ft_length_n(fmt, n, ft_strlen(base));
}

void	ft_p_to_buffer(char **buffer, t_format *format, void *p)
{
	char	*start;
	int		len;
	int		width;

	if (!p)
	{
		format->precision = -1;
		return (ft_s_to_buffer(buffer, format, "(nil)"));
	}
	width = format->width;
	start = *buffer;
	len = 0;
	len += 2;
	len += ft_num_base_to_buffer(buffer, (unsigned long)p, "0123456789abcdef");
	*(*buffer)++ = 'x';
	*(*buffer)++ = '0';
	while (width-- > len)
		*(*buffer)++ = ' ';
	if (format->left_align != -1)
		ft_reverse_substring(start, 0,
			ft_length_num_base((unsigned long)p, 16) + 1);
	else
		ft_reverse_substring(start, 0, ft_length_p(format, p) - 1);
}
