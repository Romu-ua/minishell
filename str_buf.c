/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_buf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 00:08:24 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/10 00:08:24 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>

void	sb_init(t_str_buf *b)
{
	b->s = NULL;
	b->cap = 0;
	b->len = 0;
}

void	sb_putc(t_str_buf *b, int c)
{
	size_t	ncap;
	char	*ns;

	if (b->len + 1 >= b->cap)
	{
		if (b->cap)
			ncap = b->cap * 2;
		else
			ncap = 32;
		ns = (char *)realloc(b->s, ncap * sizeof(char));
		if (!ns)
		{
			perror("realloc");
			exit(1);
		}
		b->cap = ncap;
		b->s = ns;
	}
	b->s[b->len++] = (char)c;
	b->s[b->len] = '\0';
}

char	*sb_take(t_str_buf *sb)
{
	char	*r;

	r = sb->s;
	sb->s = NULL;
	sb->cap = 0;
	sb->len = 0;
	return (r);
}

void	sb_free(t_str_buf *b)
{
	free(b->s);
}
