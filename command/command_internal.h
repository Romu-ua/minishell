/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:03:00 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:03:00 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_INTERNAL_H
# define COMMAND_INTERNAL_H

# include "exec.h"

struct s_stdio
{
	int	in;
	int	out;
};

int		cmd_join_path(char *dst, size_t dstsz,
			const char *dir, const char *cmd);
void	cmd_exec_external(char **argv);
int		cmd_restore_stdio(int saved_in, int saved_out);

#endif
