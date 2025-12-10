/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:07:57 by tomvaroux         #+#    #+#             */
/*   Updated: 2025/12/10 15:12:25 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

// typedef struct s_env
// {
// 	char			*name;
// 	char			*value;
// 	int				exported;
// 	struct s_env	*next;
// }	t_env;

static t_env	**env_head(void);

static t_env	*env_new(const char *name, const char *value, int exported);
static t_env	*env_find(const char *name);
static void		env_add_back(t_env *node);

void			env_clear(void);
void			env_init(char **envp);
const char		*env_get(const char *name);
int				env_set(const char *name, const char *value, int exported);
void			env_unset(const char *name);
char			**env_to_envp(void);
void			env_free_envp(char **envp);


/* 内部の env リストへの入口（関数内 static） */

static t_env	**env_head(void)
{
	static t_env	*head = NULL;

	return (&head);
}

static t_env	*env_new(const char *name, const char *value, int exported)
{
	t_env	*e;

	e = (t_env *)calloc(1, sizeof(t_env));
	if (!e)
		fatal("calloc");
	e->name = strdup(name ? name : "");
	if (!e->name)
		fatal("strdup");
	if (value)
	{
		e->value = strdup(value);
		if (!e->value)
			fatal("strdup");
	}
	e->exported = exported;
	e->next = NULL;
	return (e);
}

static t_env	*env_find(const char *name)
{
	t_env	*cur;

	cur = *env_head();
	while (cur)
	{
		if (strcmp(cur->name, name) == 0)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

static void	env_add_back(t_env *node)
{
	t_env	**headp;
	t_env	*cur;

	headp = env_head();
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

/* すべて破棄 */

void	env_clear(void)
{
	t_env	**headp;
	t_env	*cur;
	t_env	*next;

	headp = env_head();
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

/* ==== 公開 API ==== */

void	env_init(char **envp)
{
	char	*eq;
	char	*name;
	char	*value;
	int		i;

	env_clear();
	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		eq = strchr(envp[i], '=');
		if (eq)
		{
			name = strndup(envp[i], eq - envp[i]);
			value = strdup(eq + 1);
			if (!name || !value)
				fatal("strdup");
			env_add_back(env_new(name, value, 1));
			free(name);
			free(value);
		}
		else
			env_add_back(env_new(envp[i], NULL, 1));
		i++;
	}
}

const char	*env_get(const char *name)
{
	t_env	*e;

	e = env_find(name);
	if (!e)
		return (NULL);
	return (e->value);
}

int	env_set(const char *name, const char *value, int exported)
{
	t_env	*e;

	if (!name || !*name)
		return (-1);
	e = env_find(name);
	if (!e)
	{
		env_add_back(env_new(name, value, exported));
		return (0);
	}
	if (e->value)
	{
		free(e->value);
		e->value = NULL;
	}
	if (value)
	{
		e->value = strdup(value);
		if (!e->value)
			fatal("strdup");
	}
	if (exported)
		e->exported = 1;
	return (0);
}

void	env_unset(const char *name)
{
	t_env	**headp;
	t_env	*cur;
	t_env	*prev;

	headp = env_head();
	prev = NULL;
	cur = *headp;
	while (cur)
	{
		if (strcmp(cur->name, name) == 0)
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

/* ★ここが env_to_envp： exported なら value が NULL でも出す */

char	**env_to_envp(void)
{
	int		count;
	t_env	*cur;
	char	**envp;
	int		i;
	size_t	len;
	char	*entry;

	count = 0;
	cur = *env_head();
	while (cur)
	{
		if (cur->exported)
			count++;
		cur = cur->next;
	}
	envp = (char **)calloc(count + 1, sizeof(char *));
	if (!envp)
		fatal("calloc");
	cur = *env_head();
	i = 0;
	while (cur)
	{
		if (cur->exported)
		{
			len = strlen(cur->name) + 1
				+ (cur->value ? strlen(cur->value) : 0);
			entry = (char *)malloc(len + 1);
			if (!entry)
				fatal("malloc");
			strcpy(entry, cur->name);
			strcat(entry, "=");
			if (cur->value)
				strcat(entry, cur->value);
			envp[i++] = entry;
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
