/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:06:41 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:06:42 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static int	var_is_start(int c)
{
	return (ms_isalpha((unsigned char)c) || c == '_');
}

static int	var_is_body(int c)
{
	return (ms_isalnum((unsigned char)c) || c == '_');
}

static int	dollar_status(t_exp *e)
{
	if (tv_sb_put_status(e->b, (int)g_exit_status) < 0)
		return (-1);
	e->i += 2;
	return (0);
}

static int	dollar_name(t_exp *e)
{
	size_t		start;
	size_t		end;
	size_t		nlen;
	char		name[256];
	const char	*val;

	start = e->i + 1;
	end = start;
	while (var_is_body((unsigned char)e->src[end]))
		end++;
	nlen = end - start;
	if (nlen >= sizeof(name))
		nlen = sizeof(name) - 1;
	ms_memcpy(name, e->src + start, nlen);
	name[nlen] = '\0';
	val = env_get(name);
	if (val && tv_sb_puts(e->b, val) < 0)
		return (-1);
	e->i = end;
	return (0);
}

int	exp_dollar(t_exp *e)
{
	char	next;

	next = e->src[e->i + 1];
	if (next == '\0')
		return (tv_sb_putc(e->b, '$'), e->i++, 0);
	if (next == '?')
		return (dollar_status(e));
	if (var_is_start((unsigned char)next))
		return (dollar_name(e));
	if (tv_sb_putc(e->b, '$') < 0)
		return (-1);
	e->i++;
	return (0);
}
