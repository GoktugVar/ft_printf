/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivar <ivar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 08:43:22 by ivar              #+#    #+#             */
/*   Updated: 2024/11/02 13:43:12 by ivar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

typedef struct s_format
{
	int		width;
	int		precision;
	int		left_align;
	int		zero_padding;
	int		alternate_form;
	int		space;
	int		plus;
	char	specifier;
}	t_format;

#pragma region [PARSE]
void	init_format(t_format *format)
{
	format->alternate_form = 0;
	format->left_align = -1;
	format->plus = 0;
	format->precision = -1;
	format->space = 0;
	format->specifier = 0;
	format->width = 0;
	format->zero_padding = 0;
}

void	parse_flags(const char **str, t_format *format)
{
	while (**str == '#' || **str == '-' || **str == '+'
		|| **str == ' ' || **str == '0')
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
			format->zero_padding = 1;
		(*str)++;
	}
}

void	parse_width(const char **str, t_format *format, va_list args)
{
	if (**str == '*')
	{
		format->width = va_arg(args, int);
		(*str)++;
	}
	else
	{
		format->width = 0;
		while (**str >= '0' && **str <= '9')
			format->width = format->width * 10 + (*((*str)++) - '0');
	}
}

void	parse_precision(const char **str, t_format *format, va_list args)
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
				format->precision = format->precision * 10
					+ (*((*str)++) - '0');
		}
	}
	else
		format->precision = -1;
}

void	parse_specifier(const char **str, t_format *format)
{
	if (**str == 'c' || **str == 's' || **str == 'p' || **str == 'd' || **str == '%'
		|| **str == 'i' || **str == 'u' || **str == 'x' || **str == 'X')
		format->specifier = *((*str)++);
}
#pragma endregion

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
		i++;
	return (i);
}

#pragma region [FT_Length]
int	ft_length_c(t_format *format, int c)
{
	(void) c;
	if (format->width)
		return (format->width);
	return (1);
}

int	ft_length_num_base(long nbr, char *base, int base_len)
{
	int	len;

	len = 1;
	if (nbr <= 0)
		nbr *= -1;
	else
		len = 0;
	while (nbr != 0)
	{
		nbr /= base_len;
		len++;
	}
	return (len);
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

int	ft_length_n(t_format *format, long nbr, char *base, int base_len)
{
	int	len;

	len = ft_length_num_base(nbr, base, base_len);
	if (len < format->precision)
		len = format->precision;
	if (len < format->width)
		len = format->width;
	return (len);
}

int	ft_length_p(t_format *format, void *p)
{
	int	len;

	len = ft_length_num_base((unsigned long)p, "123456789abcdef", 16);
	if (format->precision > len)
		len = format->precision;
	len += 2;
	if (format->width > len)
		len = format->width;
	return (len);
}
#pragma endregion

int	ft_lenght_arg(t_format *format, va_list args)
{
	if (format->specifier == 'c')
		return (ft_length_c(format, va_arg(args, int)));
	else if (format->specifier == 's')
		return (ft_length_s(format, va_arg(args, char *)));
	else if (format->specifier == 'p')
		return (ft_length_p(format, va_arg(args, void *)));
	else if (format->specifier == 'd' || format->specifier == 'i')
		return (ft_length_n(format, va_arg(args, int),
				"0123456789", 10));
	else if (format->specifier == 'u')
		return (ft_length_n(format, va_arg(args, unsigned int),
				"0123456789", 10));
	else if (format->specifier == 'x')
		return (ft_length_n(format, va_arg(args, unsigned int),
				"0123456789abcdef", 16));
	else if (format->specifier == 'X')
		return (ft_length_n(format, va_arg(args, unsigned int),
				"0123456789ABCDEF", 16));
	else if (format->specifier == '%')
		return (1);
	return (-1);
}

int	get_format_len(const char *str, va_list args)
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
			parse_width(&str, &format, args);
			parse_precision(&str, &format, args);
			parse_specifier(&str, &format);
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

#pragma region [To_Buffer]
void	ft_c_to_buffer(char **buffer, t_format *format, int c)
{
	if (format->left_align == -1)
		while (format->width-- > 1)
			*(*buffer)++ = ' ';
	*(*buffer)++ = (char)c;
	if (format->left_align == 1 && ++(format->width) > 1)
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

void	ft_num_base_to_buffer(char **buffer, long nbr, char *base, int base_len)
{
	if (nbr < 0)
	{
		*(*buffer)++ = '-';
		nbr = -nbr;
	}
	if (nbr >= base_len)
		ft_num_base_to_buffer(buffer, nbr / base_len, base, base_len);
	*(*buffer)++ = base[nbr % base_len];
}

void	ft_n_to_buffer(char **buffer, t_format *format, long nbr, char *base, int base_len)
{
	int len = 0;
	int is_negative = (nbr < 0 && format->specifier == 'd');
	if (is_negative)
	{
		nbr = -nbr;
		len++;
	}
	len += ft_length_n(format, nbr, base, base_len);
	if (format->left_align == -1)
	{
		while (format->width-- > len)
			*(*buffer)++ = (format->zero_padding ? '0' : ' ');
	}
	if (is_negative)
		*(*buffer)++ = '-';
	ft_num_base_to_buffer(buffer, nbr, base, base_len);
	if (format->left_align == 1)
	{
		while (format->width-- > len)
			*(*buffer)++ = ' ';
	}
}

void	ft_p_to_buffer(char **buffer, t_format *format, void *p)
{
	unsigned long addr = (unsigned long)p;
	int len = ft_length_p(format, p);

	if (format->left_align == -1)
	{
		while (format->width-- > len)
			*(*buffer)++ = ' ';
	}
	*(*buffer)++ = '0';
	*(*buffer)++ = 'x';
	ft_num_base_to_buffer(buffer, addr, "0123456789abcdef", 16);

	if (format->left_align == 1)
	{
		while (format->width-- > len)
			*(*buffer)++ = ' ';
	}
}

#pragma endregion

void	arg_to_buffer(char **buffer, t_format *format, va_list args)
{
	if (format->specifier == 'c')
		ft_c_to_buffer(buffer, format, va_arg(args, int));
	else if (format->specifier == 's')
		ft_s_to_buffer(buffer, format, va_arg(args, char *));
	else if (format->specifier == 'p')
		ft_p_to_buffer(buffer, format, va_arg(args, void *));
	else if (format->specifier == 'd' || format->specifier == 'i')
		ft_n_to_buffer(buffer, format, va_arg(args, int),
				"0123456789", 10);
	else if (format->specifier == 'u')
		ft_n_to_buffer(buffer, format, va_arg(args, unsigned int),
				"0123456789", 10);
	else if (format->specifier == 'x')
		ft_n_to_buffer(buffer, format, va_arg(args, unsigned int),
				"0123456789abcdef", 16);
	else if (format->specifier == 'X')
		ft_n_to_buffer(buffer, format, va_arg(args, unsigned int),
				"0123456789ABCDEF", 16);
	else if(format->specifier == '%')
		*(*buffer)++ = '%';
}

void	set_buffer(const char *str, char *buffer, va_list args)
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
	buffer = (char *)malloc(len);
	if (!buffer)
		return (-1);
	va_start(args, str);
	set_buffer(str, buffer, args);
	if (write(1, buffer, len) == -1)
		return (va_end(args), free(buffer), -1);
	return (va_end(args), free(buffer), len);
}
