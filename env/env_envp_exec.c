/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_envp_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:03:58 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:03:58 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	count_exec_envp(void)
{
	int		n;
	t_env	*cur;

	n = 0;
	cur = env_list();
	while (cur)
	{
		if (cur->exported && cur->value)
			n++;
		cur = cur->next;
	}
	return (n);
}

static void	free_partial(char **envp, int used)
{
	int	i;

	i = 0;
	while (i < used)
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

static char	*join_kv(const char *k, const char *v)
{
	size_t	kl;
	size_t	vl;
	char	*s;

	kl = ms_strlen(k);
	vl = ms_strlen(v);
	s = (char *)malloc(kl + 1 + vl + 1);
	if (!s)
		return (NULL);
	ms_memcpy(s, k, kl);
	s[kl] = '=';
	ms_memcpy(s + kl + 1, v, vl);
	s[kl + 1 + vl] = '\0';
	return (s);
}

char	**env_to_envp_exec(void)
{
	int		n;
	int		i;
	char	**envp;
	t_env	*cur;

	n = count_exec_envp();
	envp = (char **)ms_calloc((size_t)n + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	cur = env_list();
	while (cur)
	{
		if (cur->exported && cur->value)
		{
			envp[i] = join_kv(cur->name, cur->value);
			if (!envp[i])
				return (free_partial(envp, i), NULL);
			i++;
		}
		cur = cur->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	env_free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
