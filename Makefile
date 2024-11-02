DOTC = ft_printf.c
   
NAME = libftprintf.a
OBJECTS = $(DOTC:.c=.o)

all: $(NAME)

$(NAME):
	@$(CC) $(CFLAGS) -c $(DOTC)
	@ar rcs $@ $(OBJECTS)

%.o: %.c
	@$(CC) -c $(CFLAGS) $?

clean:
	@rm -f $(OBJECTS)	

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re