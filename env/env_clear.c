/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_clear.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:03:52 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:03:52 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

t_env	**env_head_ref(void);

void	env_clear(void)
{
	t_env	*cur;
	t_env	*next;
	t_env	**headp;

	headp = env_head_ref();
	cur = *headp;
	while (cur)
	{
		next = cur->next;
		free(cur->name);
		free(cur->value);
		free(cur);
		cur = next;
	}
	*headp = NULL;
}
