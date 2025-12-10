/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:40:53 by tomvaroux         #+#    #+#             */
/*   Updated: 2025/12/10 13:58:44 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <readline/readline.h>
#include <readline/history.h>


static char	*ms_strndup(const char *s, size_t n);

static int	open_in(const char *path);
static int	open_out_trunc(const char *path);
static int	open_out_app(const char *path);

static int	handle_heredoc(t_redir *r);

int			apply_redirs(t_redir *r);


static char	*ms_strndup(const char *s, size_t n)
{
	char	*r;

	r = (char *)malloc(n + 1);
	if (!r)
		return (NULL);
	memcpy(r, s, n);
	r[n] = '\0';
	return (r);
}

static int	open_in(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		perror(path);
	return (fd);
}

static int	open_out_trunc(const char *path)
{
	int	fd;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		perror(path);
	return (fd);
}

static int	open_out_app(const char *path)
{
	int	fd;

	fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		perror(path);
	return (fd);
}

/* heredoc: delimiter のクォートで展開有無を決める */

static int	handle_heredoc(t_redir *r)
{
	int		pfd[2];
	char	*line;
	char	*delim;
	int		expand;
	size_t	len;

	if (pipe(pfd) < 0)
	{
		perror("pipe");
		return (-1);
	}
	expand = 1;
	delim = NULL;
	len = strlen(r->target);
	if (len >= 2 && (r->target[0] == '\'' || r->target[0] == '"')
		&& r->target[len - 1] == r->target[0])
	{
		expand = 0;
		delim = ms_strndup(r->target + 1, len - 2);
	}
	else
		delim = strdup(r->target);
	if (!delim)
	{
		close(pfd[0]);
		close(pfd[1]);
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		if (expand)
		{
			char	*ex;

			ex = expand_heredoc_line(line);
			if (!ex)
			{
				free(line);
				break ;
			}
			write(pfd[1], ex, strlen(ex));
			write(pfd[1], "\n", 1);
			free(ex);
			free(line);
		}
		else
		{
			write(pfd[1], line, strlen(line));
			write(pfd[1], "\n", 1);
			free(line);
		}
	}
	free(delim);
	close(pfd[1]);
	if (dup2(pfd[0], STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(pfd[0]);
		return (-1);
	}
	close(pfd[0]);
	return (0);
}

int	apply_redirs(t_redir *r)
{
	int	fd;

	while (r)
	{
		if (r->type == R_IN)
		{
			fd = open_in(r->target);
			if (fd < 0)
				return (-1);
			if (dup2(fd, STDIN_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				return (-1);
			}
			close(fd);
		}
		else if (r->type == R_OUT_TRUNC)
		{
			fd = open_out_trunc(r->target);
			if (fd < 0)
				return (-1);
			if (dup2(fd, STDOUT_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				return (-1);
			}
			close(fd);
		}
		else if (r->type == R_OUT_APP)
		{
			fd = open_out_app(r->target);
			if (fd < 0)
				return (-1);
			if (dup2(fd, STDOUT_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				return (-1);
			}
			close(fd);
		}
		else if (r->type == R_HERDOC)
		{
			if (handle_heredoc(r) < 0)
				return (-1);
		}
		r = r->next;
	}
	return (0);
}
