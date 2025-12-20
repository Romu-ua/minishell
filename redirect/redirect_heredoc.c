/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:10:51 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:10:52 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_internal.h"
#include <readline/readline.h>
#include <readline/history.h>

static int	hd_open_pipe(struct s_hd *h)
{
	h->pfd[0] = -1;
	h->pfd[1] = -1;
	if (pipe(h->pfd) < 0)
		return (-1);
	return (0);
}

static int	hd_set_delim(struct s_hd *h, const char *target)
{
	size_t	len;

	h->expand = 1;
	len = ms_strlen(target);
	if (len >= 2 && (target[0] == '\'' || target[0] == '"')
		&& target[len - 1] == target[0])
	{
		h->expand = 0;
		h->delim = ms_strndup(target + 1, len - 2);
	}
	else
		h->delim = ms_strdup(target);
	if (!h->delim)
		return (-1);
	return (0);
}

static int	hd_write_one(struct s_hd *h, const char *line)
{
	char	*ex;

	ex = NULL;
	if (h->expand)
	{
		ex = expand_heredoc_line(line);
		if (!ex)
			return (-1);
		write(h->pfd[1], ex, ms_strlen(ex));
		free(ex);
	}
	else
		write(h->pfd[1], line, ms_strlen(line));
	write(h->pfd[1], "\n", 1);
	return (0);
}

static int	hd_read_loop(struct s_hd *h)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ms_strcmp(line, h->delim) == 0)
		{
			free(line);
			break ;
		}
		if (hd_write_one(h, line) < 0)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
	return (0);
}

int	rd_handle_heredoc(t_redir *r)
{
	struct s_hd	h;

	h.delim = NULL;
	if (hd_open_pipe(&h) < 0)
		return (-1);
	if (hd_set_delim(&h, r->target) < 0)
		return (close(h.pfd[0]), close(h.pfd[1]), -1);
	if (hd_read_loop(&h) < 0)
	{
		free(h.delim);
		close(h.pfd[0]);
		close(h.pfd[1]);
		return (-1);
	}
	free(h.delim);
	close(h.pfd[1]);
	if (dup2(h.pfd[0], STDIN_FILENO) < 0)
	{
		close(h.pfd[0]);
		return (-1);
	}
	close(h.pfd[0]);
	return (0);
}
