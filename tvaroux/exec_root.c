/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_root.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:38:48 by tomvaroux         #+#    #+#             */
/*   Updated: 2025/12/10 16:21:33 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

// #include "../main.h"	//	exec_astのg_exit_status用

void		fatal(const char *msg);
int			is_empty_command(t_cmd *cmd);

static int	exec_and_node(t_ast *node);
static int	exec_or_node(t_ast *node);

int	exec_ast(t_ast *node);


void	fatal(const char *msg)
{
	perror(msg);
	exit(1);
}

int	is_empty_command(t_cmd *cmd)
{
	return (!cmd->argv || !cmd->argv[0]);
}

/* AND / OR ノード */

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

/* AST 実行のエントリポイント */

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
		fprintf(stderr, "minishell: internal error: unknown node kind\n");
		st = 1;
	}
	g_exit_status = st;
	return (st);
}
