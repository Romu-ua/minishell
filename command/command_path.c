/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:03:04 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:03:05 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_internal.h"

int	cmd_join_path(char *dst, size_t dstsz,
		const char *dir, const char *cmd)
{
	size_t	i;
	size_t	j;

	if (!dst || dstsz == 0 || !dir || !cmd)
		return (-1);
	i = 0;
	while (dir[i] && i + 2 < dstsz)
	{
		dst[i] = dir[i];
		i++;
	}
	if (i + 2 >= dstsz)
		return (-1);
	dst[i++] = '/';
	j = 0;
	while (cmd[j] && i + 1 < dstsz)
		dst[i++] = cmd[j++];
	if (i + 1 >= dstsz)
		return (-1);
	dst[i] = '\0';
	return (0);
}
