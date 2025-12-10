/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:39:53 by tomvaroux         #+#    #+#             */
/*   Updated: 2025/12/10 15:18:32 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"


static void	exec_external(char **argv);

void		exec_command_child(t_cmd *cmd);
int			exec_single_command(t_cmd *cmd);


/* PATH からコマンド探索 & execve */

static void	exec_external(char **argv)
{
	char	*cmd;
	char	*path;
	char	*dup;
	char	*start;
	char	*end;
	char	**envp;
	char	buf[4096];

	cmd = argv[0];
	if (!cmd)
		exit(0);
	envp = env_to_envp();
	if (strchr(cmd, '/'))
	{
		execve(cmd, argv, envp);
		perror(cmd);
		env_free_envp(envp);
		exit(127);
	}
	path = (char *)env_get("PATH");
	if (!path)
		path = getenv("PATH");
	if (!path)
	{
		execve(cmd, argv, envp);
		perror(cmd);
		env_free_envp(envp);
		exit(127);
	}
	dup = strdup(path);
	if (!dup)
		fatal("strdup");
	start = dup;
	while (start && *start)
	{
		end = strchr(start, ':');
		if (end)
			*end = '\0';
		snprintf(buf, sizeof(buf), "%s/%s", start, cmd);
		execve(buf, argv, envp);
		if (!end)
			break ;
		start = end + 1;
	}
	fprintf(stderr, "minishell: %s: command not found\n", cmd);
	free(dup);
	env_free_envp(envp);
	exit(127);
}

/* 子プロセスで 1 コマンド実行（パイプライン用） */

void	exec_command_child(t_cmd *cmd)
{
	int	st;

	if (apply_redirs(cmd->redirs) < 0)
		exit(1);
	if (is_empty_command(cmd))
		exit(0);
	if (is_builtin(cmd->argv[0]))
	{
		st = exec_builtin(cmd->argv);
		exit(st);
	}
	exec_external(cmd->argv);
}

/* パイプじゃない 1 コマンド実行（親） */

int	exec_single_command(t_cmd *cmd)
{
	int		saved_stdin;
	int		saved_stdout;
	int		status;
	pid_t	pid;

	if (is_empty_command(cmd))
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdin < 0 || saved_stdout < 0)
			fatal("dup");
		if (apply_redirs(cmd->redirs) < 0)
		{
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
			return (1);
		}
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (0);
	}
	if (is_builtin(cmd->argv[0]))
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdin < 0 || saved_stdout < 0)
			fatal("dup");
		if (apply_redirs(cmd->redirs) < 0)
		{
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
			return (1);
		}
		status = exec_builtin(cmd->argv);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (status);
	}
	pid = fork();
	if (pid < 0)
		fatal("fork");
	if (pid == 0)
		exec_command_child(cmd);
	if (waitpid(pid, &status, 0) < 0)
		fatal("waitpid");
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
