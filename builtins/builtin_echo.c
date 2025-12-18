/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:00:34 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:00:38 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

int	bi_echo(char **argv)
{
	int	i;
	int	newline;
	int	first;

	i = 1;
	newline = 1;
	if (argv[1] && ms_strncmp(argv[1], "-n", 3) == 0)
	{
		newline = 0;
		i = 2;
	}
	first = 1;
	while (argv[i])
	{
		if (!first)
			write(STDOUT_FILENO, " ", 1);
		first = 0;
		write(STDOUT_FILENO, argv[i], ms_strlen(argv[i]));
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
