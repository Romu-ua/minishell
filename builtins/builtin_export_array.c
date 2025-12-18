/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_array.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:01:18 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:01:19 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

static int	count_exported(void)
{
	t_env	*cur;
	int		n;

	cur = env_list();
	n = 0;
	while (cur)
	{
		if (cur->exported)
			n++;
		cur = cur->next;
	}
	return (n);
}

static t_env	**alloc_env_ptrs(int n)
{
	t_env	**arr;

	arr = (t_env **)ms_calloc((size_t)n, sizeof(t_env *));
	return (arr);
}

static int	fill_exported(t_env **arr, int n)
{
	t_env	*cur;
	int		i;

	cur = env_list();
	i = 0;
	while (cur)
	{
		if (cur->exported && i < n)
			arr[i++] = cur;
		cur = cur->next;
	}
	return (i);
}

t_env	**bi_export_array(int *out_n)
{
	t_env	**arr;
	int		n;
	int		filled;

	n = count_exported();
	*out_n = n;
	if (n <= 0)
		return (NULL);
	arr = alloc_env_ptrs(n);
	if (!arr)
		return (NULL);
	filled = fill_exported(arr, n);
	*out_n = filled;
	return (arr);
}

void	bi_export_sort(t_env **arr, int n)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < n)
	{
		j = i + 1;
		while (j < n)
		{
			if (ms_strcmp(arr[i]->name, arr[j]->name) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}
