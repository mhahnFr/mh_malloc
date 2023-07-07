SRCS = $(shell find . -name \*.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))
DEPS = $(patsubst %.c, %.d, $(SRCS))

CFLAGS = -Wall -Wextra
LDFLAGS =

NAME = libmalloc.so

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $(NAME) $(OBJS)
	
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<
	
clean:
	- $(RM) -f $(OBJS)
	- $(RM) -f $(DEPS)
	
fclean: clean
	- $(RM) -f $(NAME)
	
re: fclean
	$(MAKE) all
	
.PHONY: clean fclean re all

-include $(DEPS)
