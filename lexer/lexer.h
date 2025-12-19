/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:09:03 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:09:04 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
# include <stdlib.h>
# include <stdio.h>

typedef enum e_token_kind
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
}	t_token_kind;

typedef struct s_token
{
	t_token_kind	kind;
	char			*lexeme;
}	t_token;

typedef struct s_token_vec
{
	t_token	*data;
	size_t	size;
	size_t	cap;
}	t_token_vec;

typedef struct s_str_buf
{
	char	*s;
	size_t	len;
	size_t	cap;
}	t_str_buf;

int		lex_line(const char *line, t_token_vec *out);
void	tv_init(t_token_vec *v);
void	tv_push(t_token_vec *v, t_token t);
void	tv_free(t_token_vec *v);
void	sb_init(t_str_buf *b);
void	sb_putc(t_str_buf *b, int c);
char	*sb_take(t_str_buf *sb);
void	sb_free(t_str_buf *b);
void	skip_space(const char **p);
int		is_opchar(int c);
int		read_quote(const char **p, t_str_buf *b, int quote);
int		read_word(const char **p, t_str_buf *b);

#endif
