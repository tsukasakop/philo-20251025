NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCDIR = .
SOURCES = main.c philo_routine.c monitor.c utils.c init.c time_utils.c parse_utils.c

OBJECTS = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus:
	$(MAKE) -C ../philo_bonus

test: $(NAME)
	@echo "\n=== Philosophers Test Suite (Vitest) ==="
	@echo "Testing mandatory part..."
	@cd ../test && npm run test:mandatory

.PHONY: all clean fclean re bonus test
