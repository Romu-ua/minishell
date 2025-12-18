/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:09:29 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:09:30 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token	*peek(t_parser *ps)
{
	return (&ps->tv->data[ps->pos]);
}

t_token	*advance(t_parser *ps)
{
	return (&ps->tv->data[ps->pos++]);
}

int	match(t_parser *ps, t_token_kind k)
{
	if (peek(ps)->kind == k)
	{
		ps->pos++;
		return (1);
	}
	return (0);
}

int	expect(t_parser *ps, t_token_kind k, const char *msg)
{
	if (match(ps, k))
		return (1);
	ps->err = msg;
	return (0);
}
