#
# mh_malloc - Small memory management library.
#
# Copyright (C) 2023  mhahnFr
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this library, see the file LICENSE.  If not, see <https://www.gnu.org/licenses/>.
#

TEST_DIR  = ./test

SRCS = $(shell find . -name \*.c \! -path $(TEST_DIR)\*)
OBJS = $(patsubst %.c, %.o, $(SRCS))
DEPS = $(patsubst %.c, %.d, $(SRCS))

CFLAGS = -std=gnu11 -Wall -Wextra -fPIC -Ofast
LDFLAGS = -shared -fPIC

NAME = libmalloc.so

TEST_NAME  = test.exe
TEST_SRCS  = $(shell find $(TEST_DIR) -name \*.c)
TEST_OBJS  = $(patsubst %.c, %.o, $(TEST_SRCS))
TEST_DEPS  = $(patsubst %.c, %.d, $(TEST_SRCS))
TEST_CLEAN = test_clean.exe

ifeq ($(shell uname -s),Linux)
	TEST_LDFLAGS = -Wl,-rpath=.
	TIME_SHELL = /bin/bash
else
	TEST_LDFLAGS =
	TIME_SHELL = /bin/sh
endif

all: $(NAME)

run: $(TEST_NAME)
	./$(TEST_NAME) 0 1 2

bench: SHELL := $(TIME_SHELL)
bench: $(TEST_NAME) $(TEST_CLEAN)
	@echo "\033[32mBenchmarking small allocations (First: Yours, second: STD)\033[0m"
	@for i in `seq 1 10`; do time ./$(TEST_NAME) 0; echo; time ./$(TEST_CLEAN) 0; echo; echo "\033[1m-----\033[0m"; done
	@echo
	@echo "\033[32mBenchmarking medium allocations (First: Yours, second: STD)\033[0m"
	@for i in `seq 1 10`; do time ./$(TEST_NAME) 1; echo; time ./$(TEST_CLEAN) 1; echo; echo "\033[1m-----\033[0m"; done
	@echo
	@echo "\033[32mBenchmarking large allocations (First: Yours, second: STD)\033[0m"
	@for i in `seq 1 10`; do time ./$(TEST_NAME) 2; echo; time ./$(TEST_CLEAN) 2; echo; echo "\033[1m-----\033[0m"; done

$(TEST_CLEAN): $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o $@

$(TEST_NAME): $(NAME) $(TEST_OBJS)
	$(CC) $(TEST_LDFLAGS) $(TEST_OBJS) $(NAME) -o $@

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $(NAME) $(OBJS)
	
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<
	
clean:
	- $(RM) $(OBJS)
	- $(RM) $(TEST_OBJS)
	- $(RM) $(DEPS)
	- $(RM) $(TEST_DEPS)
	
fclean: clean
	- $(RM) $(NAME)
	- $(RM) $(TEST_CLEAN)
	- $(RM) $(TEST_NAME)
	
re: fclean
	$(MAKE) all
	
.PHONY: clean fclean re all run bench

-include $(DEPS)
-include $(TEST_DEPS)
