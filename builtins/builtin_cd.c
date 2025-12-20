/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:00:18 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:00:19 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

static int	cd_update_pwds(struct s_cd *c)
{
	const char	*oval;

	oval = NULL;
	if (c->oldpwd[0])
		oval = c->oldpwd;
	if (env_set("OLDPWD", oval, 1) < 0)
		return (1);
	if (env_set("PWD", c->newpwd, 1) < 0)
		return (1);
	return (0);
}

static int	cd_to(struct s_cd *c)
{
	if (!c->path)
		return (1);
	if (chdir(c->path) < 0)
		return (1);
	if (getcwd(c->newpwd, sizeof(c->newpwd)))
		return (cd_update_pwds(c));
	return (0);
}

static int	cd_dash(struct s_cd *c)
{
	c->path = env_get("OLDPWD");
	if (!c->path)
	{
		ms_putstr_fd("minishell: cd: OLDPWD not set\n", MS_STDERR);
		return (1);
	}
	if (cd_to(c) != 0)
		return (1);
	write(STDOUT_FILENO, c->newpwd, ms_strlen(c->newpwd));
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}

static int	cd_choose_path(char **argv, struct s_cd *c)
{
	if (argv[1] && ms_strncmp(argv[1], "-", 2) == 0 && !argv[2])
		return (cd_dash(c));
	if (!argv[1])
		c->path = env_get("HOME");
	else
		c->path = argv[1];
	if (!c->path)
	{
		ms_putstr_fd("minishell: cd: HOME not set\n", MS_STDERR);
		return (1);
	}
	return (cd_to(c));
}

int	bi_cd(char **argv)
{
	struct s_cd	c;

	if (!getcwd(c.oldpwd, sizeof(c.oldpwd)))
		c.oldpwd[0] = '\0';
	c.newpwd[0] = '\0';
	c.path = NULL;
	return (cd_choose_path(argv, &c));
}
