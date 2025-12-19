/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:04:26 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:04:26 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

t_env	**env_head_ref(void);

void	env_unset(const char *name)
{
	t_env	**headp;
	t_env	*cur;
	t_env	*prev;

	headp = env_head_ref();
	prev = NULL;
	cur = *headp;
	while (cur)
	{
		if (ms_strcmp(cur->name, name) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*headp = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}
