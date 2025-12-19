/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_stdio.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:03:17 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:03:17 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_internal.h"

int	cmd_restore_stdio(int saved_in, int saved_out)
{
	int	ok;

	ok = 0;
	if (dup2(saved_in, STDIN_FILENO) < 0)
		ok = -1;
	if (dup2(saved_out, STDOUT_FILENO) < 0)
		ok = -1;
	close(saved_in);
	close(saved_out);
	return (ok);
}
