SRCS = $(shell find . -name \*.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))
DEPS = $(patsubst %.c, %.d, $(SRCS))

CFLAGS = -Wall -Wextra -fPIC -g
LDFLAGS = -shared -fPIC

NAME = libmalloc.so

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $(NAME) $(OBJS)
	
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<
	
clean:
	- $(RM) $(OBJS)
	- $(RM) $(DEPS)
	
fclean: clean
	- $(RM) $(NAME)
	
re: fclean
	$(MAKE) all
	
.PHONY: clean fclean re all

-include $(DEPS)
