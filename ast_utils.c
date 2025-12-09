/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 00:02:40 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/10 00:02:41 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

t_ast	*ast_new(t_node_kind k)
{
	t_ast	*n;

	n = (t_ast *)calloc(1, sizeof(t_ast));
	if (!n)
	{
		perror("calloc");
		exit(1);
	}
	n->kind = k;
	return (n);
}

void	free_ast(t_ast *n)
{
	if (!n)
		return ;
	free_ast(n->lhs);
	free_ast(n->rhs);
	if (n->kind == N_COMMAND)
	{
		free_argv(n->cmd.argv);
		redir_free(n->cmd.redirs);
	}
	free(n);
}
