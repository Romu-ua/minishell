/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 00:07:33 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/10 00:07:36 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ast	*parser_pipeline(t_parser *ps)
{
	t_ast	*node;
	t_ast	*rhs;
	t_ast	*p;

	node = parser_command(ps);
	if (!node)
		return (NULL);
	while (peek(ps)->kind == T_PIPE)
	{
		advance(ps);
		rhs = parser_command(ps);
		if (!rhs)
		{
			free_ast(node);
			return (NULL);
		}
		p = ast_new(N_PIPE);
		p->rhs = rhs;
		p->lhs = node;
		node = p;
	}
	return (node);
}

static t_ast	*combine_and_or(t_parser *ps, t_ast *node)
{
	t_ast		*rhs;
	t_ast		*p;
	t_node_kind	nk;

	if (peek(ps)->kind == T_AND_IF)
		nk = N_AND;
	else
		nk = N_OR;
	advance(ps);
	rhs = parser_pipeline(ps);
	if (!rhs)
	{
		free_ast(node);
		return (NULL);
	}
	p = ast_new(nk);
	p->rhs = rhs;
	p->lhs = node;
	return (p);
}

t_ast	*parser_and_or(t_parser *ps)
{
	t_ast	*node;

	node = parser_pipeline(ps);
	if (!node)
		return (NULL);
	while (peek(ps)->kind == T_OR_IF || peek(ps)->kind == T_AND_IF)
	{
		node = combine_and_or(ps, node);
		if (!node)
			return (NULL);
	}
	return (node);
}

t_ast	*parser_input(t_parser *ps)
{
	t_ast	*root;

	root = parser_and_or(ps);
	if (!root)
		return (NULL);
	if (peek(ps)->kind != T_EOF)
	{
		ps->err = "unexpected token after experssion";
		free_ast(root);
		return (NULL);
	}
	return (root);
}
