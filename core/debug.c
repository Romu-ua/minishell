/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:03:39 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:03:40 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdio.h>

static char	*rname(t_redir_type t)
{
	if (t == R_IN)
		return ("<");
	if (t == R_OUT_TRUNC)
		return (">");
	if (t == R_OUT_APP)
		return (">>");
	if (t == R_HERDOC)
		return ("<<");
	return ("?");
}

static void	print_redirs(t_redir *r, int depth)
{
	int	j;

	while (r)
	{
		j = 0;
		while (j < depth + 2)
		{
			putchar(' ');
			j++;
		}
		printf("(REDIR %s \"%s\")\n", rname(r->type), r->target);
		r = r->next;
	}
}

static void	print_cmd(t_ast *n, int depth)
{
	size_t	i;

	printf("(CMD");
	if (n->cmd.argv)
	{
		i = 0;
		while (n->cmd.argv[i])
		{
			printf(" \"%s\"", n->cmd.argv[i]);
			i++;
		}
	}
	printf(")\n");
	print_redirs(n->cmd.redirs, depth);
}

static void	print_binary_node(const char *name, t_ast *n, int depth)
{
	puts(name);
	print_ast(n->lhs, depth + 2);
	print_ast(n->rhs, depth + 2);
}

void	print_ast(t_ast *n, int depth)
{
	int	i;

	if (!n)
		return ;
	i = 0;
	while (i < depth)
	{
		putchar(' ');
		i++;
	}
	if (n->kind == N_PIPE)
		print_binary_node("(PIPE)", n, depth);
	else if (n->kind == N_AND)
		print_binary_node("(&&)", n, depth);
	else if (n->kind == N_OR)
		print_binary_node("(||)", n, depth);
	else if (n->kind == N_COMMAND)
		print_cmd(n, depth);
}
