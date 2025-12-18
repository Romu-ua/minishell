/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:01:09 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:01:10 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

static int	exit_badnum(char *arg)
{
	ms_putstr_fd("minishell: exit: ", MS_STDERR);
	ms_putstr_fd(arg, MS_STDERR);
	ms_putstr_fd(": numeric argument required\n", MS_STDERR);
	ms_set_exit(255);
	return (255);
}

static int	exit_toomany(void)
{
	ms_putstr_fd("minishell: exit: too many arguments\n", MS_STDERR);
	return (1);
}

int	bi_exit(char **argv)
{
	int		ok;
	long	code;

	write(STDERR_FILENO, "exit\n", 5);
	if (!argv[1])
	{
		ms_set_exit((int)g_exit_status);
		return ((int)g_exit_status);
	}
	code = bi_exit_atol(argv[1], &ok);
	if (!ok)
		return (exit_badnum(argv[1]));
	if (argv[2])
		return (exit_toomany());
	ms_set_exit((unsigned char)code);
	return ((unsigned char)code);
}
