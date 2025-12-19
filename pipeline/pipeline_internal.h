/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_internal.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:10:01 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:10:01 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPELINE_INTERNAL_H
# define PIPELINE_INTERNAL_H

# include "exec.h"

typedef struct s_cmd_list
{
	t_cmd	**data;
	size_t	size;
	size_t	cap;
}	t_cmd_list;

typedef struct s_pipe_ctx
{
	t_cmd_list	cl;
	int			n;
	int			pipes_n;
	int			(*pipes)[2];
	pid_t		*pids;
	int			spawned;
	int			last_status;
}	t_pipe_ctx;

int		exec_pipeline_node(t_ast *pipe_root);

void	cl_init(t_cmd_list *cl);
int		cl_push(t_cmd_list *cl, t_cmd *cmd);
int		flatten_pipeline(t_ast *node, t_cmd_list *cl);

int		pipes_make(int (*pipes)[2], int n);
void	pipes_init(int (*pipes)[2], int n);
void	pipes_close(int (*pipes)[2], int n);

void	wait_spawned(pid_t *pids, int spawned);
int		wait_last_status(pid_t *pids, int n);

#endif
