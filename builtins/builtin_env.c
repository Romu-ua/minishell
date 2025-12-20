/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:00:48 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:00:49 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

int	bi_env(char **argv)
{
	char	**envp;
	int		i;

	(void)argv;
	envp = env_to_envp_exec();
	if (!envp)
		return (1);
	i = 0;
	while (envp[i])
	{
		write(STDOUT_FILENO, envp[i], ms_strlen(envp[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	env_free_envp(envp);
	return (0);
}
