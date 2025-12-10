/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 00:07:08 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/10 17:26:15 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "signal.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"

#include "tvaroux/exec.h"

volatile sig_atomic_t	g_exit_status;

// int	main(void)
// {
// 	char	*line;
// 	t_ast	*root;

// 	g_exit_status = 0;
// 	setup_signal();
// 	while (1)
// 	{
// 		line = readline("minishell> ");
// 		if (!line || (strcmp(line, "exit") == 0))
// 		{
// 			write(1, "exit\n", 5);
// 			break ;
// 		}
// 		root = ast(line);
// 		if (!root)
// 		{
// 			free_ast(root);
// 			continue ;
// 		}
// 		print_ast(root, 0);
// 		free_ast(root);
// 		free(line);
// 	}
// 	return (0);
// }

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_ast	*root;

	g_exit_status = 0;
	setup_signal();

	(void)argc;	//
	(void)argv;	//
	env_init(envp);	//
	rl_outstream = stderr;

	while (1)
	{
		// line = readline("minishell> ");
		line = readline("minishell$ ");	//
		// if (!line || (strcmp(line, "exit") == 0))
		// {
		// 	write(2, "exit\n", 5);
		// 	break ;
		// }
		if (!line)
		{
			write(2, "exit\n", 5);
			break ;
		}
		if (*line)	//
			add_history(line);	//
		root = ast(line);
		if (!root)
		{
			g_exit_status = 258; /* syntax error */
			free_ast(root);
			continue ;
		}
		// print_ast(root, 0);


		expand_ast(root);	//
		exec_ast(root);	//
		if (ms_should_exit())
		{
			// int status = (int)g_exit_status;
			free_ast(root);
			free(line);
			env_clear();
			rl_clear_history();
			exit((int)g_exit_status);
		}

		free_ast(root);
		free(line);
	}
	env_clear();
	rl_clear_history();
	return ((int)g_exit_status);
}
