NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

UNAME := $(shell uname)

INCLUDES = -Iinclude -Icore -Iexec -Ibuiltins -Ienv -Iexpand -Iutils -Ipipeline -Iredirect -Icommand -Ilexer -Iparser -Iast
LIBS = -lreadline

# macの場合はbrew installしたreadlineの場所を示す必要がある。
ifeq ($(UNAME), Darwin)
    RL_DIR = /opt/homebrew/opt/readline
    INCLUDES += -I$(RL_DIR)/include
    LIBS = -L$(RL_DIR)/lib -lreadline
endif

SRCS_DIR_TV	=	core exec builtins env expand utils pipeline redirect command lexer parser ast
SRCS_TV		=	$(wildcard $(addsuffix /*.c, $(SRCS_DIR_TV)))
SRCS		= $(SRCS_TV)
OBJS = $(SRCS:.c=.o)

all: $(NAME)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
clean:
	rm -f $(OBJS)
fclean: clean
	rm -f $(NAME)
re: fclean all
.PHONY: all clean fclean re
