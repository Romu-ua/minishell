/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:00:26 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:00:27 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

static t_bi_ent	*bi_table(void)
{
	static t_bi_ent	tbl[] = {
	{"echo", &bi_echo},
	{"cd", &bi_cd},
	{"pwd", &bi_pwd},
	{"export", &bi_export},
	{"unset", &bi_unset},
	{"env", &bi_env},
	{"exit", &bi_exit},
	{NULL, NULL}
	};

	return (tbl);
}

static int	bi_find_index(const char *cmd)
{
	t_bi_ent	*tbl;
	int			i;

	if (!cmd)
		return (-1);
	tbl = bi_table();
	i = 0;
	while (tbl[i].name)
	{
		if (ms_strcmp(cmd, tbl[i].name) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	is_builtin(const char *cmd)
{
	if (bi_find_index(cmd) < 0)
		return (0);
	return (1);
}

int	exec_builtin(char **argv)
{
	t_bi_ent	*tbl;
	int			idx;

	if (!argv || !argv[0])
		return (0);
	idx = bi_find_index(argv[0]);
	if (idx < 0)
		return (1);
	tbl = bi_table();
	return (tbl[idx].fn(argv));
}
