/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:01:29 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:01:30 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

static void	export_print_one(t_env *e)
{
	write(STDOUT_FILENO, "declare -x ", 11);
	write(STDOUT_FILENO, e->name, ms_strlen(e->name));
	if (e->value != NULL)
	{
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, e->value, ms_strlen(e->value));
		write(STDOUT_FILENO, "\"", 1);
	}
	write(STDOUT_FILENO, "\n", 1);
}

static void	export_print_unsorted(void)
{
	t_env	*cur;

	cur = env_list();
	while (cur)
	{
		if (cur->exported)
			export_print_one(cur);
		cur = cur->next;
	}
}

void	bi_print_export(void)
{
	t_env	**arr;
	int		n;
	int		i;

	n = 0;
	arr = bi_export_array(&n);
	if (n <= 0)
		return ;
	if (!arr)
	{
		export_print_unsorted();
		return ;
	}
	bi_export_sort(arr, n);
	i = 0;
	while (i < n)
		export_print_one(arr[i++]);
	free(arr);
}
