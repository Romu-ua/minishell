/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_sv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:07:23 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:07:24 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

void	tv_sv_init(t_str_vec *v)
{
	v->data = NULL;
	v->size = 0;
	v->cap = 0;
}

int	tv_sv_push(t_str_vec *v, char *s)
{
	char	**nd;
	size_t	ncap;
	size_t	oldsz;
	size_t	newsz;

	if (v->size == v->cap)
	{
		ncap = 8;
		if (v->cap > 0)
			ncap = v->cap * 2;
		oldsz = v->cap * sizeof(char *);
		newsz = ncap * sizeof(char *);
		nd = (char **)ms_realloc(v->data, oldsz, newsz);
		if (!nd)
			return (-1);
		v->data = nd;
		v->cap = ncap;
	}
	v->data[v->size] = s;
	v->size++;
	return (0);
}

char	**tv_sv_take_null_terminated(t_str_vec *v)
{
	char	**r;

	if (tv_sv_push(v, NULL) < 0)
		return (NULL);
	r = v->data;
	v->data = NULL;
	v->size = 0;
	v->cap = 0;
	return (r);
}

void	tv_sv_free_all(t_str_vec *v)
{
	size_t	i;

	i = 0;
	while (i < v->size)
	{
		free(v->data[i]);
		i++;
	}
	free(v->data);
	v->data = NULL;
	v->size = 0;
	v->cap = 0;
}

void	tv_free_argv(char **argv)
{
	size_t	i;

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
