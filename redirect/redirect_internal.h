/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_internal.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:10:59 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:11:00 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_INTERNAL_H
# define REDIRECT_INTERNAL_H

# include "exec.h"

struct s_hd
{
	int		pfd[2];
	char	*delim;
	int		expand;
};

int	rd_open_in(const char *path);
int	rd_open_out_trunc(const char *path);
int	rd_open_out_app(const char *path);
int	rd_handle_heredoc(t_redir *r);

#endif
