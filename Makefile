NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
# macはreadlineを設定しないといけないのでこれが必要
RL_DIR = /opt/homebrew/opt/readline
INCLUDES = -I$(RL_DIR)/include
LIBS = -L$(RL_DIR)/lib -lreadline

# SRCS = main.c ast.c ast_utils.c lexer.c lexer_utils.c lexer_word.c token_vec.c str_buf.c \
# 	   parser.c parser_command.c parser_utils.c redir.c str_vec.c debug.c signal.c

SRCS_HY	= main.c ast.c ast_utils.c lexer.c lexer_utils.c lexer_word.c token_vec.c str_buf.c \
		parser.c parser_command.c parser_utils.c redir.c str_vec.c debug.c signal.c

SRCS_DIR_TV	=	tvaroux

SRCS_TV		=	$(wildcard $(addsuffix /*.c, $(SRCS_DIR_TV)))

SRCS		= $(SRCS_HY) $(SRCS_TV)

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
