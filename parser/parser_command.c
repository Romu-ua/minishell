/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:09:22 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:09:22 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>
#include <string.h>

static int	parse_args(t_parser *ps, t_str_vec *argv, t_redir **rlist)
{
	const t_token	*t;
	int				progress;

	while (peek(ps)->kind == T_WORD)
	{
		t = advance(ps);
		sv_push(argv, strdup(t->lexeme));
		while (1)
		{
			progress = parser_redir(ps, rlist);
			if (progress != 1)
				break ;
		}
		if (progress == -1)
			return (-1);
	}
	return (0);
}

static void	cleanup_command(t_str_vec *argv, t_redir *rlist)
{
	size_t	i;

	i = 0;
	while (i < argv->size)
	{
		free(argv->data[i]);
		i++;
	}
	free(argv->data);
	redir_free(rlist);
}

static int	parse_prefix_redirs(t_parser *ps, t_redir **rlist)
{
	int	progress;

	while (1)
	{
		progress = parser_redir(ps, rlist);
		if (progress != 1)
			break ;
	}
	return (progress);
}

static t_ast	*create_command_node(t_str_vec *argv, t_redir *rlist)
{
	t_ast	*n;

	n = ast_new(N_COMMAND);
	n->cmd.argv = sv_take_null_terminated(argv);
	n->cmd.redirs = rlist;
	return (n);
}

t_ast	*parser_command(t_parser *ps)
{
	t_str_vec	argv;
	t_redir		*rlist;
	int			progress;

	rlist = NULL;
	sv_init(&argv);
	progress = parse_prefix_redirs(ps, &rlist);
	if (progress == -1 || parse_args(ps, &argv, &rlist) == -1)
	{
		ps->err = "invalid redirection";
		cleanup_command(&argv, rlist);
		return (NULL);
	}
	if (!rlist && argv.size == 0)
	{
		ps->err = "expected command or redirection";
		cleanup_command(&argv, rlist);
		return (NULL);
	}
	return (create_command_node(&argv, rlist));
}
