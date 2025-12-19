/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:04:14 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:04:15 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

t_env	**env_head_ref(void);

static int	env_add_kv(const char *name, const char *value)
{
	if (env_set(name, value, 1) < 0)
		return (-1);
	return (0);
}

static int	env_add_entry(const char *s)
{
	char	*eq;
	char	*name;
	int		ret;

	eq = ms_strchr(s, '=');
	if (!eq)
		return (env_add_kv(s, NULL));
	name = ms_strndup(s, (size_t)(eq - s));
	if (!name)
		return (-1);
	ret = env_add_kv(name, eq + 1);
	free(name);
	return (ret);
}

int	env_init(char **envp)
{
	int	i;

	env_clear();
	if (!envp)
		return (0);
	i = 0;
	while (envp[i])
	{
		if (env_add_entry(envp[i]) < 0)
			return (env_clear(), -1);
		i++;
	}
	return (0);
}
