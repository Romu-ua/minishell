/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_wait.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:10:33 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:10:34 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline_internal.h"

void	wait_spawned(pid_t *pids, int spawned)
{
	int	i;
	int	st;

	if (!pids)
		return ;
	i = 0;
	while (i < spawned)
	{
		if (pids[i] > 0)
			waitpid(pids[i], &st, 0);
		i++;
	}
}

int	wait_last_status(pid_t *pids, int n)
{
	int	i;
	int	st;
	int	last;

	last = 1;
	i = 0;
	while (i < n)
	{
		if (waitpid(pids[i], &st, 0) < 0)
			last = 1;
		if (i == n - 1)
		{
			if (WIFEXITED(st))
				last = WEXITSTATUS(st);
			else if (WIFSIGNALED(st))
				last = 128 + WTERMSIG(st);
			else
				last = 1;
		}
		i++;
	}
	return (last);
}
