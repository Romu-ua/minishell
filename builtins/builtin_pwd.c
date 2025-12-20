/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:02:02 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:02:03 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

int	bi_pwd(char **argv)
{
	char	buf[4096];

	(void)argv;
	if (!getcwd(buf, sizeof(buf)))
		return (1);
	write(STDOUT_FILENO, buf, ms_strlen(buf));
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}
