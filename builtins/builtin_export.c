/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:01:40 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:01:41 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

static void	export_err(const char *arg)
{
	ms_putstr_fd("minishell: export: `", MS_STDERR);
	ms_putstr_fd(arg, MS_STDERR);
	ms_putstr_fd("': not a valid identifier\n", MS_STDERR);
}

static int	export_apply(char *name, char *value)
{
	if (!bi_is_identifier(name))
	{
		export_err(name);
		return (1);
	}
	if (env_set(name, value, 1) < 0)
	{
		perror("malloc");
		return (1);
	}
	return (0);
}

static int	export_set_one(char *arg)
{
	char	*eq;
	char	*value;
	int		st;

	eq = ms_strchr(arg, '=');
	value = NULL;
	if (eq)
	{
		*eq = '\0';
		value = eq + 1;
	}
	st = export_apply(arg, value);
	if (eq)
		*eq = '=';
	return (st);
}

int	bi_export(char **argv)
{
	int	i;
	int	st;

	if (!argv[1])
	{
		bi_print_export();
		return (0);
	}
	i = 1;
	st = 0;
	while (argv[i])
	{
		if (export_set_one(argv[i]) != 0)
			st = 1;
		i++;
	}
	return (st);
}
