/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:02:52 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:02:53 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_internal.h"

static void	exec_not_found(char *cmd, char **envp, char *dup)
{
	ms_putstr_fd("minishell: ", MS_STDERR);
	ms_putstr_fd(cmd, MS_STDERR);
	ms_putstr_fd(": command not found\n", MS_STDERR);
	free(dup);
	env_free_envp(envp);
	exit(127);
}

static void	exec_with_slash(char *cmd, char **argv, char **envp)
{
	execve(cmd, argv, envp);
	env_free_envp(envp);
	exit(127);
}

static void	exec_path_loop(char *cmd, char **argv, char **envp, char *dup)
{
	char	*start;
	char	*end;
	char	buf[4096];

	start = dup;
	while (*start)
	{
		end = ms_strchr(start, ':');
		if (end)
			*end = '\0';
		if (cmd_join_path(buf, sizeof(buf), start, cmd) == 0)
			execve(buf, argv, envp);
		if (!end)
			break ;
		start = end + 1;
	}
	exec_not_found(cmd, envp, dup);
}

static void	exec_from_path(char *cmd, char **argv,
							char **envp, const char *path)
{
	char	*dup;

	dup = ms_strdup(path);
	if (!dup)
	{
		env_free_envp(envp);
		exit(1);
	}
	exec_path_loop(cmd, argv, envp, dup);
}

void	cmd_exec_external(char **argv)
{
	char		**envp;
	const char	*path;
	char		*cmd;

	cmd = argv[0];
	if (!cmd)
		exit(0);
	envp = env_to_envp_exec();
	if (!envp)
		exit(1);
	setup_signal_exec();
	if (ms_strchr(cmd, '/'))
		exec_with_slash(cmd, argv, envp);
	path = env_get("PATH");
	if (!path)
		exec_not_found(cmd, envp, NULL);
	exec_from_path(cmd, argv, envp, path);
}
