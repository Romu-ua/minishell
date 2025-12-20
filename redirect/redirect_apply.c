/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_apply.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:10:46 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:10:47 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_internal.h"

static int	rd_dup_to(int fd, int stdfd)
{
	if (dup2(fd, stdfd) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	rd_apply_one(t_redir *r)
{
	int	fd;

	fd = -1;
	if (r->type == R_IN)
		fd = rd_open_in(r->target);
	else if (r->type == R_OUT_TRUNC)
		fd = rd_open_out_trunc(r->target);
	else if (r->type == R_OUT_APP)
		fd = rd_open_out_app(r->target);
	else if (r->type == R_HERDOC)
		return (rd_handle_heredoc(r));
	if (fd < 0)
		return (-1);
	if (r->type == R_IN)
		return (rd_dup_to(fd, STDIN_FILENO));
	return (rd_dup_to(fd, STDOUT_FILENO));
}

int	apply_redirs(t_redir *r)
{
	while (r)
	{
		if (rd_apply_one(r) < 0)
			return (-1);
		r = r->next;
	}
	return (0);
}
