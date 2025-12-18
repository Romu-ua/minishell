/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:06:55 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:06:55 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static void	exp_init_hd(t_exp *e, const char *src, t_str_buf *b)
{
	e->src = src;
	e->i = 0;
	e->in_s = 0;
	e->in_d = 0;
	e->b = b;
}

static int	exp_loop_hd(t_exp *e)
{
	char	c;

	while (e->src[e->i])
	{
		c = e->src[e->i];
		if (c == '$')
		{
			if (exp_dollar(e) < 0)
				return (-1);
			continue ;
		}
		if (tv_sb_putc(e->b, c) < 0)
			return (-1);
		e->i++;
	}
	return (0);
}

char	*expand_heredoc_line(const char *src)
{
	t_str_buf	b;
	t_exp		e;

	if (!src)
		return (NULL);
	tv_sb_init(&b);
	exp_init_hd(&e, src, &b);
	if (exp_loop_hd(&e) < 0)
		return (tv_sb_free(&b), NULL);
	if (!b.s)
	{
		b.s = (char *)malloc(1);
		if (!b.s)
			return (NULL);
		b.s[0] = '\0';
	}
	return (b.s);
}
