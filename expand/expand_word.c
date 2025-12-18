/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:07:35 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:07:36 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static void	exp_init(t_exp *e, const char *src, t_str_buf *b)
{
	e->src = src;
	e->i = 0;
	e->in_s = 0;
	e->in_d = 0;
	e->b = b;
}

static int	exp_loop_word(t_exp *e)
{
	char	c;

	while (e->src[e->i])
	{
		c = e->src[e->i];
		if (c == '\'' && e->in_d == 0)
			e->in_s = !e->in_s;
		else if (c == '"' && e->in_s == 0)
			e->in_d = !e->in_d;
		else if (c == '$' && e->in_s == 0)
		{
			if (exp_dollar(e) < 0)
				return (-1);
			continue ;
		}
		else if (tv_sb_putc(e->b, c) < 0)
			return (-1);
		e->i++;
	}
	return (0);
}

int	expand_word(const char *src, char **out)
{
	t_str_buf	b;
	t_exp		e;

	if (!out)
		return (-1);
	*out = NULL;
	if (!src)
		return (0);
	tv_sb_init(&b);
	exp_init(&e, src, &b);
	if (exp_loop_word(&e) < 0)
		return (tv_sb_free(&b), -1);
	if (!b.s)
	{
		b.s = (char *)malloc(1);
		if (!b.s)
			return (-1);
		b.s[0] = '\0';
	}
	*out = b.s;
	return (0);
}
