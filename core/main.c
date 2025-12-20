/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:03:26 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:03:28 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "ms_signal.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "exec.h"

volatile sig_atomic_t	g_exit_status;

static void	run_command(t_ast *root)
{
	ms_register_ast(root);
	if (expand_ast(root) < 0)
	{
		g_exit_status = 1;
		free_ast(root);
		ms_register_ast(NULL);
		return ;
	}
	exec_ast(root);
	if (ms_should_exit())
	{
		free_ast(root);
		ms_register_ast(NULL);
		env_clear();
		rl_clear_history();
		exit((int)g_exit_status);
	}
	free_ast(root);
	ms_register_ast(NULL);
}

static void	process_line(char *line)
{
	t_ast	*root;

	add_history(line);
	root = ast(line);
	free(line);
	if (!root)
	{
		g_exit_status = 258;
		return ;
	}
	run_command(root);
}

static int	init_shell(char **envp)
{
	g_exit_status = 0;
	if (env_init(envp) < 0)
		return (1);
	rl_outstream = stderr;
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argc;
	(void)argv;
	if (init_shell(envp))
		return (1);
	while (1)
	{
		setup_signal();
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		process_line(line);
	}
	write(2, "exit\n", 5);
	env_clear();
	rl_clear_history();
	return ((int)g_exit_status);
}
