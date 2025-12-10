/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:40:31 by tomvaroux         #+#    #+#             */
/*   Updated: 2025/12/10 13:59:43 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

typedef struct s_cmd_list
{
	t_cmd	**data;
	size_t	size;
	size_t	cap;
}	t_cmd_list;


static void	cl_init(t_cmd_list *cl);
static void	cl_push(t_cmd_list *cl, t_cmd *cmd);
static void	flatten_pipeline(t_ast *node, t_cmd_list *cl);

int			exec_pipeline_node(t_ast *pipe_root);


static void	cl_init(t_cmd_list *cl)
{
	cl->data = NULL;
	cl->size = 0;
	cl->cap = 0;
}

static void	cl_push(t_cmd_list *cl, t_cmd *cmd)
{
	t_cmd	**nd;
	size_t	ncap;

	if (cl->size == cl->cap)
	{
		if (cl->cap)
			ncap = cl->cap * 2;
		else
			ncap = 4;
		nd = (t_cmd **)realloc(cl->data, ncap * sizeof(t_cmd *));
		if (!nd)
			fatal("realloc");
		cl->data = nd;
		cl->cap = ncap;
	}
	cl->data[cl->size++] = cmd;
}

static void	flatten_pipeline(t_ast *node, t_cmd_list *cl)
{
	if (!node)
		return ;
	if (node->kind == N_PIPE)
	{
		flatten_pipeline(node->lhs, cl);
		flatten_pipeline(node->rhs, cl);
	}
	else if (node->kind == N_COMMAND)
		cl_push(cl, &node->cmd);
	else
		fprintf(stderr, "minishell: internal error: non-command in pipeline\n");
}

int	exec_pipeline_node(t_ast *pipe_root)
{
	t_cmd_list	cl;
	int			n;
	int			i;
	int			(*pipes)[2];
	pid_t		*pids;
	int			status;
	int			st;

	cl_init(&cl);
	flatten_pipeline(pipe_root, &cl);
	n = (int)cl.size;
	if (n == 0)
		return (0);
	if (n == 1)
	{
		st = exec_single_command(cl.data[0]);
		free(cl.data);
		return (st);
	}
	pipes = calloc(n - 1, sizeof(int[2]));
	pids = calloc(n, sizeof(pid_t));
	if (!pipes || !pids)
		fatal("calloc");
	i = 0;
	while (i < n - 1)
	{
		if (pipe(pipes[i]) < 0)
			fatal("pipe");
		i++;
	}
	i = 0;
	while (i < n)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			fatal("fork");
		if (pids[i] == 0)
		{
			if (i > 0)
			{
				if (dup2(pipes[i - 1][0], STDIN_FILENO) < 0)
					fatal("dup2");
			}
			if (i < n - 1)
			{
				if (dup2(pipes[i][1], STDOUT_FILENO) < 0)
					fatal("dup2");
			}
			for (int j = 0; j < n - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}
			exec_command_child(cl.data[i]);
			exit(1);
		}
		i++;
	}
	i = 0;
	while (i < n - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	status = 0;
	i = 0;
	while (i < n)
	{
		int	st2;

		if (waitpid(pids[i], &st2, 0) < 0)
			fatal("waitpid");
		if (i == n - 1)
		{
			if (WIFEXITED(st2))
				status = WEXITSTATUS(st2);
			else if (WIFSIGNALED(st2))
				status = 128 + WTERMSIG(st2);
			else
				status = 1;
		}
		i++;
	}
	free(pipes);
	free(pids);
	free(cl.data);
	return (status);
}
