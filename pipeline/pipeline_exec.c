/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:09:49 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:09:49 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline_internal.h"

static void	ctx_init(t_pipe_ctx *c)
{
	cl_init(&c->cl);
	c->n = 0;
	c->pipes_n = 0;
	c->pipes = NULL;
	c->pids = NULL;
	c->spawned = 0;
	c->last_status = 1;
}

static void	ctx_free(t_pipe_ctx *c)
{
	pipes_close(c->pipes, c->pipes_n);
	wait_spawned(c->pids, c->spawned);
	free(c->pipes);
	free(c->pids);
	free(c->cl.data);
}

static int	ctx_prepare(t_pipe_ctx *c, t_ast *root)
{
	if (flatten_pipeline(root, &c->cl) < 0)
		return (-1);
	c->n = (int)c->cl.size;
	if (c->n == 0)
		return (0);
	if (c->n == 1)
	{
		c->last_status = exec_single_command(c->cl.data[0]);
		return (0);
	}
	c->pipes_n = c->n - 1;
	c->pipes = (int (*)[2])ms_calloc((size_t)c->pipes_n, sizeof (int [2]));
	if (!c->pipes)
		return (perror("malloc"), -1);
	pipes_init(c->pipes, c->pipes_n);
	c->pids = (pid_t *)ms_calloc((size_t)c->n, sizeof(pid_t));
	if (!c->pids)
		return (perror("malloc"), -1);
	if (pipes_make(c->pipes, c->pipes_n) < 0)
		return (-1);
	return (1);
}

static int	ctx_spawn(t_pipe_ctx *c)
{
	int	i;

	i = 0;
	while (i < c->n)
	{
		c->pids[i] = fork();
		if (c->pids[i] < 0)
			return (perror("fork"), -1);
		if (c->pids[i] == 0)
		{
			if (i > 0 && dup2(c->pipes[i - 1][0], STDIN_FILENO) < 0)
				_exit(1);
			if (i < c->n - 1 && dup2(c->pipes[i][1], STDOUT_FILENO) < 0)
				_exit(1);
			pipes_close(c->pipes, c->pipes_n);
			exec_command_child(c->cl.data[i]);
			_exit(1);
		}
		c->spawned++;
		i++;
	}
	return (0);
}

int	exec_pipeline_node(t_ast *pipe_root)
{
	t_pipe_ctx	c;
	int			prep;

	ctx_init(&c);
	prep = ctx_prepare(&c, pipe_root);
	if (prep <= 0)
	{
		ctx_free(&c);
		if (prep < 0)
			return (1);
		return (c.last_status);
	}
	setup_signal_ignore();
	if (ctx_spawn(&c) < 0)
	{
		setup_signal();
		return (ctx_free(&c), 1);
	}
	pipes_close(c.pipes, c.pipes_n);
	c.pipes_n = 0;
	c.spawned = 0;
	c.last_status = wait_last_status(c.pids, c.n);
	setup_signal();
	ctx_free(&c);
	return (c.last_status);
}
