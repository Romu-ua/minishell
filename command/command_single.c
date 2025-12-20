/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:03:11 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:03:11 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_internal.h"

static int	run_redir_only(t_cmd *cmd)
{
	struct s_stdio	s;

	if (!cmd->redirs)
		return (0);
	s.in = dup(STDIN_FILENO);
	s.out = dup(STDOUT_FILENO);
	if (s.in < 0 || s.out < 0)
		return (1);
	if (apply_redirs(cmd->redirs) < 0)
	{
		cmd_restore_stdio(s.in, s.out);
		return (1);
	}
	cmd_restore_stdio(s.in, s.out);
	return (0);
}

static int	run_builtin_parent(t_cmd *cmd)
{
	struct s_stdio	s;
	int				st;

	s.in = dup(STDIN_FILENO);
	s.out = dup(STDOUT_FILENO);
	if (s.in < 0 || s.out < 0)
		return (1);
	if (apply_redirs(cmd->redirs) < 0)
	{
		cmd_restore_stdio(s.in, s.out);
		return (1);
	}
	st = exec_builtin(cmd->argv);
	cmd_restore_stdio(s.in, s.out);
	return (st);
}

static int	wait_one(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) < 0)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	exec_single_command(t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	if (is_empty_command(cmd))
		return (run_redir_only(cmd));
	if (is_builtin(cmd->argv[0]))
		return (run_builtin_parent(cmd));
	setup_signal_ignore();
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
		exec_command_child(cmd);
	status = wait_one(pid);
	setup_signal();
	return (status);
}
