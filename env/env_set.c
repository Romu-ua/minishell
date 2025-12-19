/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:04:20 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:04:21 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

t_env	*env_find(const char *name);
t_env	*env_new(const char *name, const char *value, int exported);
void	env_add_back(t_env *node);

const char	*env_get(const char *name)
{
	t_env	*e;

	if (!name)
		return (NULL);
	e = env_find(name);
	if (!e)
		return (NULL);
	return (e->value);
}

static int	set_value(t_env *e, const char *value)
{
	char	*dup;

	if (e->value)
	{
		free(e->value);
		e->value = NULL;
	}
	if (!value)
		return (0);
	dup = ms_strdup(value);
	if (!dup)
		return (-1);
	e->value = dup;
	return (0);
}

static int	set_new(const char *name, const char *value, int exported)
{
	t_env	*e;

	e = env_new(name, value, exported);
	if (!e)
		return (-1);
	env_add_back(e);
	return (0);
}

int	env_set(const char *name, const char *value, int exported)
{
	t_env	*e;

	if (!name || !*name)
		return (-1);
	e = env_find(name);
	if (!e)
		return (set_new(name, value, exported));
	if (set_value(e, value) < 0)
		return (-1);
	if (exported)
		e->exported = 1;
	return (0);
}
