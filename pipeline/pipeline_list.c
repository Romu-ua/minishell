/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:10:17 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:10:18 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline_internal.h"

void	cl_init(t_cmd_list *cl)
{
	cl->data = NULL;
	cl->size = 0;
	cl->cap = 0;
}

int	cl_push(t_cmd_list *cl, t_cmd *cmd)
{
	t_cmd	**nd;
	size_t	ncap;

	if (cl->size < cl->cap)
		return (cl->data[cl->size++] = cmd, 0);
	ncap = 4;
	if (cl->cap)
		ncap = cl->cap * 2;
	nd = (t_cmd **)ms_realloc(cl->data, cl->cap * sizeof(t_cmd *),
			ncap * sizeof(t_cmd *));
	if (!nd)
		return (-1);
	cl->data = nd;
	cl->cap = ncap;
	cl->data[cl->size++] = cmd;
	return (0);
}

int	flatten_pipeline(t_ast *node, t_cmd_list *cl)
{
	if (!node)
		return (0);
	if (node->kind == N_PIPE)
	{
		if (flatten_pipeline(node->lhs, cl) < 0)
			return (-1);
		if (flatten_pipeline(node->rhs, cl) < 0)
			return (-1);
		return (0);
	}
	if (node->kind == N_COMMAND)
		return (cl_push(cl, &node->cmd));
	ms_putstr_fd("minishell: internal error: non-command in pipeline\n",
		MS_STDERR);
	return (-1);
}
