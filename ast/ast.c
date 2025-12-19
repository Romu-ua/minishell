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
		tv_free(&tv);
		return (NULL);
	}
	ps = (t_parser){.tv = &tv, .pos = 0, .err = NULL};
	root = parser_input(&ps);
	tv_free(&tv);
	return (root);
}
