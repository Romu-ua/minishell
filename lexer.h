#ifndef LEXER_H
# define LEXER_H

#include <stddef.h>

typedef enum
{
	T_WORD,
	T_PIPE,
	T_OR_IF,
	T_AND_IF,
	T_LT,
	T_GT,
	T_DLT,
	T_DGT,
	T_EOF,
	T_ERROR,
}	e_token_kind;

typedef struct s_token
{
	e_token_kind	kind;
	char			*lexeme;
}	t_token;

typedef struct s_token_vec
{
	t_token	*data;
	size_t	size;
	size_t	cap;
}	t_token_vec;

// この２つは公開する
int		lex_line(const char *line, t_token_vec *out);
void	tv_free(t_token_vec *tv);


#endif