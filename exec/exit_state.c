/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:05:58 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:05:58 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	*exit_flag_ptr(void)
{
	static int	flag = 0;

	return (&flag);
}

void	ms_set_exit(int status)
{
	*exit_flag_ptr() = 1;
	g_exit_status = status;
}

int	ms_should_exit(void)
{
	return (*exit_flag_ptr() != 0);
}

void	ms_reset_exit(void)
{
	*exit_flag_ptr() = 0;
}
