/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:02:46 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:02:47 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_internal.h"

void	exec_command_child(t_cmd *cmd)
{
	int	st;

	setup_signal_exec();
	if (apply_redirs(cmd->redirs) < 0)
		exit(1);
	if (is_empty_command(cmd))
		exit(0);
	if (is_builtin(cmd->argv[0]))
	{
		st = exec_builtin(cmd->argv);
		exit(st);
	}
	cmd_exec_external(cmd->argv);
	exit(1);
}
