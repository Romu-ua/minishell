/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:08:52 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:08:53 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <ctype.h>

int	read_quote(const char **p, t_str_buf *b, int quote)
{
	while (**p && **p != quote)
	{
		sb_putc(b, **p);
		(*p)++;
	}
	if (**p != quote)
		return (-1);
	sb_putc(b, **p);
	(*p)++;
	return (0);
}

static int	read_word_quote(const char **p, t_str_buf *b)
{
	int	q;

	q = **p;
	sb_putc(b, **p);
	(*p)++;
	if (read_quote(p, b, q) != 0)
		return (-1);
	return (0);
}

static void	read_word_normal(const char **p, t_str_buf *b)
{
	while (**p && !isspace((unsigned char)**p)
		&& !is_opchar((unsigned char)**p)
		&& **p != '\'' && **p != '"')
	{
		sb_putc(b, **p);
		(*p)++;
	}
}

int	read_word(const char **p, t_str_buf *b)
{
	int	consumed;

	consumed = 0;
	while (**p && !isspace((unsigned char)**p)
		&& !is_opchar((unsigned char)**p))
	{
		if (**p == '\'' || **p == '"')
		{
			if (read_word_quote(p, b) != 0)
				return (-1);
		}
		else
			read_word_normal(p, b);
		consumed = 1;
	}
	if (consumed)
		return (0);
	return (1);
}
