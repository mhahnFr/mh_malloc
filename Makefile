TEST_DIR  = ./test

SRCS = $(shell find . -name \*.c \! -path $(TEST_DIR)\*)
OBJS = $(patsubst %.c, %.o, $(SRCS))
DEPS = $(patsubst %.c, %.d, $(SRCS))

CFLAGS = -std=gnu11 -Wall -Wextra -fPIC -g
LDFLAGS = -shared -fPIC

NAME = libmalloc.so

TEST_NAME = test.exe
TEST_SRCS = $(shell find $(TEST_DIR) -name \*.c)
TEST_OBJS = $(patsubst %.c, %.o, $(TEST_SRCS))
TEST_DEPS = $(patsubst %.c, %.d, $(TEST_SRCS))

ifeq ($(shell uname -s),Linux)
	TEST_LDFLAGS = -Wl,-rpath=.
else
	TEST_LDFLAGS =
endif

all: $(NAME)

run: $(TEST_NAME)
	./$(TEST_NAME) 0 1 2

$(TEST_NAME): $(NAME) $(TEST_OBJS)
	$(CC) $(TEST_LDFLAGS) $(TEST_OBJS) $(NAME) -o $(TEST_NAME)

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
	
.PHONY: clean fclean re all run

-include $(DEPS)
-include $(TEST_DEPS)
