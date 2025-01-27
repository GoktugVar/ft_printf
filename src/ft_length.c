/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_length.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivar <ivar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 08:43:25 by ivar              #+#    #+#             */
/*   Updated: 2025/01/19 11:42:03 by ivar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_length_c(t_format *format, int c)
{
	(void)c;
	if (format->width)
		return (format->width);
	return (1);
}

int	ft_length_s(t_format *format, char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len == -1 && format->precision < 6 && format->precision != -1)
		len = 0;
	else if (len == -1)
		len = 6;
	else if (len > format->precision && format->precision != -1)
		len = format->precision;
	if (len < format->width)
		len = format->width;
	return (len);
}

int	ft_length_num_base(unsigned long nbr, int base_len)
{
	int	len;

	len = 0;
	if (nbr == 0)
		return (1);
	while (nbr != 0)
	{
		nbr /= base_len;
		len++;
	}
	return (len);
}

int	ft_length_n(t_format *format, long nbr, int base_len)
{
	int	len;

	if (nbr < 0)
		len = ft_length_num_base(-nbr, base_len);
	else
		len = ft_length_num_base(nbr, base_len);
	if (len < format->precision)
		len = format->precision;
	if (nbr == 0 && format->precision == 0)
		len = 0;
	if (format->alternate_form == 1 && nbr != 0)
		len += 2;
	if (format->space || format->plus || nbr < 0)
		len++;
	if (len < format->width)
		len = format->width;
	return (len);
}

int	ft_length_p(t_format *format, void *p)
{
	int	len;

	if (!p)
	{
		len = ft_strlen("(nil)");
		if (format->width > len)
			len = format->width;
		return (len);
	}
	len = ft_length_num_base((unsigned long)p, 16);
	if (len < format->precision)
		len = format->precision;
	len += 2;
	if (format->width > len)
		len = format->width;
	return (len);
}
