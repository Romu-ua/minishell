/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:06:35 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:06:36 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static int	expand_argv_loop(t_cmd *cmd, t_str_vec *v)
{
	size_t	i;
	char	*expanded;

	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		if (expand_word(cmd->argv[i], &expanded) < 0)
			return (-1);
		if (tv_sv_push(v, expanded) < 0)
		{
			free(expanded);
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	expand_argv(t_cmd *cmd)
{
	t_str_vec	v;
	char		**new_argv;

	tv_sv_init(&v);
	if (expand_argv_loop(cmd, &v) < 0)
		return (tv_sv_free_all(&v), -1);
	new_argv = tv_sv_take_null_terminated(&v);
	if (!new_argv)
		return (tv_sv_free_all(&v), -1);
	tv_free_argv(cmd->argv);
	cmd->argv = new_argv;
	return (0);
}

static int	expand_redirs(t_cmd *cmd)
{
	t_redir	*r;
	char	*expanded;

	r = cmd->redirs;
	while (r)
	{
		if (r->type != R_HERDOC)
		{
			if (expand_word(r->target, &expanded) < 0)
				return (-1);
			free(r->target);
			r->target = expanded;
		}
		r = r->next;
	}
	return (0);
}

int	expand_cmd(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	if (expand_argv(cmd) < 0)
		return (-1);
	if (expand_redirs(cmd) < 0)
		return (-1);
	return (0);
}
