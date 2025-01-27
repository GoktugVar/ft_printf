NAME = libftprintf.a

DOTC = src/ft_buffer.c src/ft_length.c src/ft_parse.c src/ft_printf.c src/ft_utils.c src/ft_strlen.c

OBJECTS = $(DOTC:.c=.o)

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

bonus: $(NAME)

$(NAME): $(OBJECTS)
	@ar rc $(NAME) $(OBJECTS)
	@ranlib $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJECTS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

a: all
b: bonus
c: clean
fc: fclean
r: re

.PHONY: all bonus clean fclean re a b c fc r
