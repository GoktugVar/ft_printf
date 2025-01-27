/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivar <ivar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 08:43:18 by ivar              #+#    #+#             */
/*   Updated: 2025/01/19 12:00:10 by ivar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>

typedef struct s_format
{
	int		alternate_form;
	int		left_align;
	int		plus;
	int		precision;
	int		space;
	char	specifier;
	int		width;
	char	zero_padding;
}			t_format;

int		ft_printf(const char *str, ...);

void	init_format(t_format *format);
void	parse_flags(const char **str, t_format *format);
int		parse_width(const char **str, t_format *format, va_list args);
int		parse_precision(const char **str, t_format *format, va_list args);
int		parse_specifier(const char **str, t_format *format);

int		ft_length_c(t_format *format, int c);
int		ft_length_s(t_format *format, char *str);
int		ft_length_num_base(unsigned long nbr, int base_len);
int		ft_length_n(t_format *format, long nbr, int base_len);
int		ft_length_p(t_format *format, void *p);

void	ft_c_to_buffer(char **buffer, t_format *format, int c);
void	ft_s_to_buffer(char **buffer, t_format *format, char *str);
void	ft_n_to_buffer(char **buf, t_format *fmt, long n, char *base);
void	ft_p_to_buffer(char **buffer, t_format *format, void *p);

int		ft_strlen(char *str);
void	ft_reverse_substring(char *str, int start, int end);
int		apply_padding_and_prefix(char **buf, t_format *fmt, long n, int *len);
void	ft_format_diux(t_format *format);

#endif