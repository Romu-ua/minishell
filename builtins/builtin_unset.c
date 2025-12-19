/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:02:13 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:02:13 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

static void	unset_err(const char *arg)
{
	ms_putstr_fd("minishell: unset: `", MS_STDERR);
	ms_putstr_fd(arg, MS_STDERR);
	ms_putstr_fd("': not a valid identifier\n", MS_STDERR);
}

int	bi_unset(char **argv)
{
	int	i;
	int	st;

	i = 1;
	st = 0;
	while (argv[i])
	{
		if (!bi_is_identifier(argv[i]))
		{
			unset_err(argv[i]);
			st = 1;
		}
		else
			env_unset(argv[i]);
		i++;
	}
	return (st);
}
