/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:08:57 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:08:58 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <string.h>

static int	lex_operator(const char **p, t_token_vec *out)
{
	t_token_kind	kind;

	if (**p == '&' && (*p)[1] == '&')
		kind = T_AND_IF;
	else if (**p == '|' && (*p)[1] == '|')
		kind = T_OR_IF;
	else if (**p == '>' && (*p)[1] == '>')
		kind = T_DGT;
	else if (**p == '<' && (*p)[1] == '<')
		kind = T_DLT;
	else
		return (0);
	tv_push(out, (t_token){.kind = kind, .lexeme = NULL});
	*p += 2;
	return (1);
}

static int	lex_single_operator(const char **p, t_token_vec *out)
{
	t_token_kind	kind;

	if (**p == '&')
	{
		tv_push(out, (t_token){.kind = T_ERROR,
			.lexeme = strdup("unexpected single '&'")});
		tv_push(out, (t_token){.kind = T_EOF, .lexeme = NULL});
		return (-1);
	}
	if (**p == '|')
		kind = T_PIPE;
	else if (**p == '>')
		kind = T_GT;
	else if (**p == '<')
		kind = T_LT;
	else
		return (0);
	tv_push(out, (t_token){.kind = kind, .lexeme = NULL});
	(*p)++;
	return (1);
}

static int	lex_word(const char **p, t_token_vec *out)
{
	t_str_buf	sb;
	int			r;

	sb_init(&sb);
	r = read_word(p, &sb);
	if (r == -1)
	{
		sb_free(&sb);
		tv_push(out, (t_token){.kind = T_ERROR,
			.lexeme = strdup("unclosed quote")});
		tv_push(out, (t_token){.kind = T_EOF, .lexeme = NULL});
		return (-1);
	}
	if (r == 1)
	{
		sb_free(&sb);
		tv_push(out, (t_token){.kind = T_ERROR,
			.lexeme = strdup("unexpected error")});
		tv_push(out, (t_token){.kind = T_EOF, .lexeme = NULL});
		return (-1);
	}
	tv_push(out, (t_token){.kind = T_WORD, .lexeme = sb_take(&sb)});
	return (0);
}

int	lex_line(const char *line, t_token_vec *out)
{
	const char	*p;
	int			ret;

	p = line;
	tv_init(out);
	while (1)
	{
		skip_space(&p);
		if (*p == '\0' || *p == '\n')
		{
			tv_push(out, (t_token){.kind = T_EOF, .lexeme = NULL});
			return (0);
		}
		if (lex_operator(&p, out))
			continue ;
		ret = lex_single_operator(&p, out);
		if (ret == -1)
			return (-1);
		if (ret == 1)
			continue ;
		if (lex_word(&p, out) == -1)
			return (-1);
	}
}
