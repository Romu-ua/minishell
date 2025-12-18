/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:10:25 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:10:25 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline_internal.h"

void	pipes_init(int (*pipes)[2], int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pipes[i][0] = -1;
		pipes[i][1] = -1;
		i++;
	}
}

int	pipes_make(int (*pipes)[2], int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pipe(pipes[i]) < 0)
		{
			perror("pipe");
			return (-1);
		}
		i++;
	}
	return (0);
}

void	pipes_close(int (*pipes)[2], int n)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < n)
	{
		if (pipes[i][0] >= 0)
			close(pipes[i][0]);
		if (pipes[i][1] >= 0)
			close(pipes[i][1]);
		i++;
	}
}
