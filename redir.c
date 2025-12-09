/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 00:07:53 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/10 00:07:53 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

t_redir	*redir_new(t_redir_type ty, const char *target)
{
	t_redir	*r;

	r = (t_redir *)calloc(1, sizeof(t_redir));
	if (!r)
	{
		perror("calloc");
		exit(1);
	}
	r->type = ty;
	if (target)
		r->target = strdup(target);
	else
		r->target = strdup("");
	return (r);
}

void	redir_push_back(t_redir **head, t_redir *node)
{
	t_redir	*cur;

	if (!*head)
	{
		*head = node;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

void	redir_free(t_redir *r)
{
	t_redir	*nx;

	while (r)
	{
		nx = r->next;
		free(r->target);
		free(r);
		r = nx;
	}
}

int	parser_redir(t_parser *ps, t_redir **out_head)
{
	t_token_kind	k;
	t_redir_type	rt;
	const t_token	*t;

	k = peek(ps)->kind;
	if (k == T_LT)
		rt = R_IN;
	else if (k == T_GT)
		rt = R_OUT_TRUNC;
	else if (k == T_DGT)
		rt = R_OUT_APP;
	else if (k == T_DLT)
		rt = R_HERDOC;
	else
		return (0);
	advance(ps);
	if (!expect(ps, T_WORD, "redir expects WORD on the right"))
		return (-1);
	t = &ps->tv->data[ps->pos - 1];
	redir_push_back(out_head, redir_new(rt, t->lexeme));
	return (1);
}
