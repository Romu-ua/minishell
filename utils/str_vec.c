/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_vec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:12:27 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:12:27 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

void	sv_init(t_str_vec *v)
{
	v->data = NULL;
	v->cap = 0;
	v->size = 0;
}

void	sv_push(t_str_vec *v, char *s)
{
	char	**nd;
	size_t	ncap;

	if (v->size == v->cap)
	{
		if (v->cap)
			ncap = v->cap * 2;
		else
			ncap = 8;
		nd = (char **)realloc(v->data, ncap * sizeof(char *));
		if (!nd)
		{
			perror("realloc");
			exit(1);
		}
		v->cap = ncap;
		v->data = nd;
	}
	v->data[v->size++] = s;
}

char	**sv_take_null_terminated(t_str_vec *v)
{
	char	**r;

	sv_push(v, NULL);
	r = v->data;
	v->data = NULL;
	v->cap = 0;
	v->size = 0;
	return (r);
}

void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}
