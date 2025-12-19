/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_setutils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:04:03 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:04:03 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

t_env	**env_head_ref(void);

t_env	*env_find(const char *name)
{
	t_env	*cur;

	cur = *env_head_ref();
	while (cur)
	{
		if (ms_strcmp(cur->name, name) == 0)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

t_env	*env_new(const char *name, const char *value, int exported)
{
	t_env	*e;
	char	*nn;

	e = (t_env *)ms_calloc(1, sizeof(t_env));
	if (!e)
		return (NULL);
	if (!name)
		name = "";
	nn = ms_strdup(name);
	if (!nn)
		return (free(e), NULL);
	e->name = nn;
	if (value)
	{
		e->value = ms_strdup(value);
		if (!e->value)
			return (free(e->name), free(e), NULL);
	}
	e->exported = exported;
	return (e);
}

void	env_add_back(t_env *node)
{
	t_env	**headp;
	t_env	*cur;

	headp = env_head_ref();
	if (!*headp)
	{
		*headp = node;
		return ;
	}
	cur = *headp;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}
