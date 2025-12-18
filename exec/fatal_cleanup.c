/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fatal_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:06:03 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:06:04 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parser.h"
#include <readline/readline.h>
#include <readline/history.h>

t_ast	**ast_slot_ref(void);

static void	cleanup_ast(void)
{
	t_ast	**slot;
	t_ast	*root;

	slot = ast_slot_ref();
	root = *slot;
	if (root)
	{
		free_ast(root);
		*slot = NULL;
	}
}

static void	cleanup_all(void)
{
	cleanup_ast();
	env_clear();
	rl_clear_history();
}

void	ms_cleanup_and_exit(int status)
{
	cleanup_all();
	exit(status);
}

void	fatal(const char *msg)
{
	perror(msg);
	ms_cleanup_and_exit(1);
}
