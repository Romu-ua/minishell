/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 15:52:10 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 15:59:54 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parser.h"
#include "lexer.h"
#include "ms_utils.h"

static int	check_syntax(t_token_vec *tv)
{
	size_t	i;
	t_token	*tok;

	i = 0;
	while (i < tv->size)
	{
		tok = &tv->data[i];
		if (tok->kind == T_EOF)
			break ;
		if (tok->kind == T_PIPE || tok->kind == T_OR_IF || tok->kind == T_AND_IF
			|| tok->kind == T_LT || tok->kind == T_GT || tok->kind == T_DLT
			|| tok->kind == T_DGT)
		{
			if (i + 1 < tv->size && tv->data[i + 1].kind == T_EOF)
			{
				ms_putstr_fd("bash: syntax error near unexpected \
						token `newline'\n", 2);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static void	check_tv_err(t_token_vec *tv)
{
	size_t	i;

	i = 0;
	while (i < tv->size)
	{
		if (tv->data[i].kind == T_ERROR)
		{
			ms_putstr_fd("bash: ", 2);
			ms_putstr_fd(tv->data[i].lexeme, 2);
			ms_putstr_fd("\n", 2);
			break ;
		}
		i++;
	}
}

t_ast	*ast(char *line)
{
	t_token_vec	tv;
	int			ok;
	t_parser	ps;
	t_ast		*root;

	if (lex_line(line, &tv) == 0)
		ok = 1;
	else
		ok = 0;
	if (!ok)
	{
		check_tv_err(&tv);
		tv_free(&tv);
		return (NULL);
	}
	if (check_syntax(&tv))
	{
		tv_free(&tv);
		return (NULL);
	}
	ps = (t_parser){.tv = &tv, .pos = 0, .err = NULL};
	root = parser_input(&ps);
	tv_free(&tv);
	return (root);
}
