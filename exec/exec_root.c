/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_root.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:05:30 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:05:30 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	exec_and_node(t_ast *node)
{
	int	st;

	st = exec_ast(node->lhs);
	if (st == 0)
		st = exec_ast(node->rhs);
	return (st);
}

static int	exec_or_node(t_ast *node)
{
	int	st;

	st = exec_ast(node->lhs);
	if (st != 0)
		st = exec_ast(node->rhs);
	return (st);
}

int	is_empty_command(t_cmd *cmd)
{
	if (!cmd->argv || !cmd->argv[0])
		return (1);
	return (0);
}

int	exec_ast(t_ast *node)
{
	int	st;

	if (!node)
		return (0);
	if (node->kind == N_COMMAND)
		st = exec_single_command(&node->cmd);
	else if (node->kind == N_PIPE)
		st = exec_pipeline_node(node);
	else if (node->kind == N_AND)
		st = exec_and_node(node);
	else if (node->kind == N_OR)
		st = exec_or_node(node);
	else
	{
		ms_putstr_fd("minishell: internal error: unknown node kind\n",
			MS_STDERR);
		st = 1;
	}
	g_exit_status = st;
	return (st);
}
