/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_sb.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:07:18 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:07:18 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

void	tv_sb_init(t_str_buf *b)
{
	b->s = NULL;
	b->len = 0;
	b->cap = 0;
}

void	tv_sb_free(t_str_buf *b)
{
	free(b->s);
	b->s = NULL;
	b->len = 0;
	b->cap = 0;
}

int	tv_sb_reserve(t_str_buf *b, size_t add)
{
	size_t	ncap;
	char	*ns;

	if (b->len + add + 1 <= b->cap)
		return (0);
	ncap = 32;
	if (b->cap > 0)
		ncap = b->cap * 2;
	while (b->len + add + 1 > ncap)
		ncap *= 2;
	ns = (char *)ms_realloc(b->s, b->cap, ncap);
	if (!ns)
		return (-1);
	b->s = ns;
	b->cap = ncap;
	return (0);
}

int	tv_sb_putc(t_str_buf *b, char c)
{
	if (tv_sb_reserve(b, 1) < 0)
		return (-1);
	b->s[b->len] = c;
	b->len++;
	b->s[b->len] = '\0';
	return (0);
}

int	tv_sb_puts(t_str_buf *b, const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = ms_strlen(s);
	if (tv_sb_reserve(b, len) < 0)
		return (-1);
	ms_memcpy(b->s + b->len, s, len);
	b->len += len;
	b->s[b->len] = '\0';
	return (0);
}
