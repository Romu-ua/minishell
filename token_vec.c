/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_vec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 00:08:44 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/10 00:08:44 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>

void	tv_init(t_token_vec *v)
{
	v->data = NULL;
	v->cap = 0;
	v->size = 0;
}

void	tv_push(t_token_vec *v, t_token t)
{
	size_t	ncap;
	t_token	*nd;

	if (v->size == v->cap)
	{
		if (v->cap)
			ncap = v->cap * 2;
		else
			ncap = 8;
		nd = (t_token *)realloc(v->data, ncap * sizeof(t_token));
		if (!nd)
		{
			perror("realloc");
			exit(1);
		}
		v->data = nd;
		v->cap = ncap;
	}
	v->data[v->size++] = t;
}

void	tv_free(t_token_vec *v)
{
	size_t	i;

	i = 0;
	while (i < v->size)
	{
		if (v->data[i].kind == T_WORD || v->data[i].kind == T_ERROR)
			free(v->data[i].lexeme);
		i++;
	}
	free(v->data);
}
