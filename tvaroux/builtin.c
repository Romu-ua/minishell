/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:39:20 by tomvaroux         #+#    #+#             */
/*   Updated: 2025/12/10 15:57:41 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"


/* ===== exit 状態管理 ===== */

// typedef struct s_exit_state
// {
// 	int	flag;
// 	int	status;
// }	t_exit_state;

static int	*exit_flag_ptr(void)
{
	static int	flag = 0;

	return (&flag);
}

void	ms_set_exit(int status)
{
	int	*flag;

	flag = exit_flag_ptr();
	*flag = 1;
	g_exit_status = status;
}

int	ms_should_exit(void)
{
	return (*exit_flag_ptr() != 0);
}

void	ms_reset_exit(void)
{
	*exit_flag_ptr() = 0;
}


/* ===== 小さめのヘルパー ===== */

static int	ms_strncmp(const char *a, const char *b, size_t n)
{
	size_t	i;

	if (!a || !b)
		return (1);
	i = 0;
	while (i < n && a[i] && b[i] && a[i] == b[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)a[i] - (unsigned char)b[i]);
}

static size_t	ms_strlen(const char *s)
{
	size_t	i = 0;

	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

static int	is_identifier(const char *s)
{
	size_t	i;

	if (!s || !s[0])
		return (0);
	if (!(isalpha((unsigned char)s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!(isalnum((unsigned char)s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

/* ===== 判定 ===== */

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ms_strncmp(cmd, "echo", 5) == 0
		|| ms_strncmp(cmd, "cd", 3) == 0
		|| ms_strncmp(cmd, "pwd", 4) == 0
		|| ms_strncmp(cmd, "export", 7) == 0
		|| ms_strncmp(cmd, "unset", 6) == 0
		|| ms_strncmp(cmd, "env", 4) == 0
		|| ms_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

/* ===== echo ===== */

static int	builtin_echo(char **argv)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (argv[1] && ms_strncmp(argv[1], "-n", 3) == 0)
	{
		newline = 0;
		i = 2;
	}
	while (argv[i])
	{
		if (i > (newline ? 1 : 2))
			write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, argv[i], ms_strlen(argv[i]));
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

/* ===== pwd ===== */

static int	builtin_pwd(char **argv)
{
	char	buf[4096];
	(void)argv;
	if (!getcwd(buf, sizeof(buf)))
	{
		perror("pwd");
		return (1);
	}
	write(STDOUT_FILENO, buf, ms_strlen(buf));
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}

/* ===== cd ===== */

// static int	builtin_cd(char **argv)
// {
// 	const char	*path;
// 	char		oldpwd[4096];
// 	char		newpwd[4096];

// 	if (!getcwd(oldpwd, sizeof(oldpwd)))
// 		oldpwd[0] = '\0';
// 	if (!argv[1])
// 		path = env_get("HOME");
// 	else
// 		path = argv[1];
// 	if (!path)
// 	{
// 		write(STDERR_FILENO, "minishell: cd: HOME not set\n",
// 			ms_strlen("minishell: cd: HOME not set\n"));
// 		return (1);
// 	}
// 	if (chdir(path) < 0)
// 	{
// 		perror("minishell: cd");
// 		return (1);
// 	}
// 	if (getcwd(newpwd, sizeof(newpwd)))
// 	{
// 		env_set("OLDPWD", oldpwd[0] ? oldpwd : NULL, 1);
// 		env_set("PWD", newpwd, 1);
// 	}
// 	return (0);
// }

static int	builtin_cd(char **argv)
{
	const char	*path;
	char		oldpwd[4096];
	char		newpwd[4096];

	if (!getcwd(oldpwd, sizeof(oldpwd)))
		oldpwd[0] = '\0';

	/* cd - */
	if (argv[1] && ms_strncmp(argv[1], "-", 2) == 0 && !argv[2])
	{
		path = env_get("OLDPWD");
		if (!path)
		{
			write(STDERR_FILENO,
				"minishell: cd: OLDPWD not set\n",
				ms_strlen("minishell: cd: OLDPWD not set\n"));
			return (1);
		}
		if (chdir(path) < 0)
		{
			perror("minishell: cd");
			return (1);
		}
		if (getcwd(newpwd, sizeof(newpwd)))
		{
			env_set("OLDPWD", oldpwd[0] ? oldpwd : NULL, 1);
			env_set("PWD", newpwd, 1);
			write(STDOUT_FILENO, newpwd, ms_strlen(newpwd));
			write(STDOUT_FILENO, "\n", 1);
		}
		return (0);
	}

	/* 通常の cd / cd PATH */
	if (!argv[1])
		path = env_get("HOME");
	else
		path = argv[1];
	if (!path)
	{
		write(STDERR_FILENO,
			"minishell: cd: HOME not set\n",
			ms_strlen("minishell: cd: HOME not set\n"));
		return (1);
	}
	if (chdir(path) < 0)
	{
		perror("minishell: cd");
		return (1);
	}
	if (getcwd(newpwd, sizeof(newpwd)))
	{
		env_set("OLDPWD", oldpwd[0] ? oldpwd : NULL, 1);
		env_set("PWD", newpwd, 1);
	}
	return (0);
}

/* ===== env ===== */

static int	builtin_env(char **argv)
{
	char	**envp;
	int		i;
	(void)argv;

	envp = env_to_envp();
	i = 0;
	while (envp[i])
	{
		write(STDOUT_FILENO, envp[i], ms_strlen(envp[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	env_free_envp(envp);
	return (0);
}

// /* ===== export ===== */

// static void	print_export(void)
// {
// 	t_env	*cur;

// 	cur = g_shell.env;
// 	while (cur)
// 	{
// 		write(STDOUT_FILENO, "declare -x ", 11);
// 		write(STDOUT_FILENO, cur->name, ms_strlen(cur->name));
// 		if (cur->value)
// 		{
// 			write(STDOUT_FILENO, "=\"", 2);
// 			write(STDOUT_FILENO, cur->value, ms_strlen(cur->value));
// 			write(STDOUT_FILENO, "\"", 1);
// 		}
// 		write(STDOUT_FILENO, "\n", 1);
// 		cur = cur->next;
// 	}
// }

// static int	builtin_export(char **argv)
// {
// 	int		i;
// 	char	*eq;
// 	char	*name;
// 	char	*value;
// 	int		st;

// 	if (!argv[1])
// 	{
// 		print_export();
// 		return (0);
// 	}
// 	st = 0;
// 	i = 1;
// 	while (argv[i])
// 	{
// 		eq = strchr(argv[i], '=');
// 		if (eq)
// 		{
// 			*eq = '\0';
// 			name = argv[i];
// 			value = eq + 1;
// 		}
// 		else
// 		{
// 			name = argv[i];
// 			value = NULL;
// 		}
// 		if (!is_identifier(name))
// 		{
// 			write(STDERR_FILENO, "minishell: export: `", 21);
// 			write(STDERR_FILENO, argv[i], ms_strlen(argv[i]));
// 			write(STDERR_FILENO, "': not a valid identifier\n", 26);
// 			st = 1;
// 		}
// 		else
// 			env_set(name, value, 1);
// 		if (eq)
// 			*eq = '=';
// 		i++;
// 	}
// 	return (st);
// }

/* ==== export ==== */

static void	print_export(void)
{
	char	**envp;
	int		i;
	char	*eq;
	size_t	name_len;

	envp = env_to_envp();          // exported なものだけ "NAME=VALUE" で返ってくる
	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		eq = strchr(envp[i], '=');
		if (!eq)
		{
			// ここには基本来ないはずだけど一応保険
			write(STDOUT_FILENO, "declare -x ", 11);
			write(STDOUT_FILENO, envp[i], ms_strlen(envp[i]));
			write(STDOUT_FILENO, "\n", 1);
			i++;
			continue ;
		}
		name_len = (size_t)(eq - envp[i]);
		write(STDOUT_FILENO, "declare -x ", 11);
		write(STDOUT_FILENO, envp[i], name_len);     // NAME
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, eq + 1, ms_strlen(eq + 1)); // VALUE
		write(STDOUT_FILENO, "\"\n", 2);
		i++;
	}
	env_free_envp(envp);
}


static int	builtin_export(char **argv)
{
	int		i;
	char	*eq;
	char	*name;
	char	*value;
	int		st;

	if (!argv[1])
	{
		print_export();
		return (0);
	}
	st = 0;
	i = 1;
	while (argv[i])
	{
		eq = strchr(argv[i], '=');
		if (eq)
		{
			*eq = '\0';
			name = argv[i];
			value = eq + 1;
		}
		else
		{
			name = argv[i];
			value = NULL;
		}
		if (!is_identifier(name))
		{
			write(STDERR_FILENO, "minishell: export: `", 21);
			write(STDERR_FILENO, argv[i], ms_strlen(argv[i]));
			write(STDERR_FILENO, "': not a valid identifier\n", 26);
			st = 1;
		}
		else
			env_set(name, value, 1);
		if (eq)
			*eq = '=';
		i++;
	}
	return (st);
}


/* ===== unset ===== */

static int	builtin_unset(char **argv)
{
	int	i;
	int	st;

	i = 1;
	st = 0;
	while (argv[i])
	{
		if (!is_identifier(argv[i]))
		{
			write(STDERR_FILENO, "minishell: unset: `", 20);
			write(STDERR_FILENO, argv[i], ms_strlen(argv[i]));
			write(STDERR_FILENO, "': not a valid identifier\n", 26);
			st = 1;
		}
		else
			env_unset(argv[i]);
		i++;
	}
	return (st);
}

/* ===== exit ===== */

static long	ms_atol(const char *s, int *ok)
{
	long	sign;
	long	val;
	size_t	i;

	sign = 1;
	val = 0;
	*ok = 1;
	i = 0;
	while (s[i] == ' ' || (s[i] >= '\t' && s[i] <= '\r'))
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	if (!isdigit((unsigned char)s[i]))
	{
		*ok = 0;
		return (0);
	}
	while (isdigit((unsigned char)s[i]))
	{
		val = val * 10 + (s[i] - '0');
		i++;
	}
	while (s[i])
	{
		if (!isspace((unsigned char)s[i]))
		{
			*ok = 0;
			return (0);
		}
		i++;
	}
	return (sign * val);
}

// static int	builtin_exit(char **argv)
// {
// 	int		ok;
// 	long	code;

// 	write(STDERR_FILENO, "exit\n", 5);
// 	if (!argv[1])
// 		exit(g_shell.last_status);
// 	code = ms_atol(argv[1], &ok);
// 	if (!ok)
// 	{
// 		write(STDERR_FILENO, "minishell: exit: ", 18);
// 		write(STDERR_FILENO, argv[1], ms_strlen(argv[1]));
// 		write(STDERR_FILENO, ": numeric argument required\n", 28);
// 		exit(255);
// 	}
// 	if (argv[2])
// 	{
// 		write(STDERR_FILENO, "minishell: exit: too many arguments\n", 36);
// 		return (1);
// 	}
// 	exit((unsigned char)code);
// 	return (0);
// }

static int	builtin_exit(char **argv)
{
	int		ok;
	long	code;

	write(STDERR_FILENO, "exit\n", 5);
	if (!argv[1])
	{
		/* 引数なし：今の g_exit_status で終了 */
		ms_set_exit((int)g_exit_status);
		return ((int)g_exit_status);
	}
	code = ms_atol(argv[1], &ok);
	if (!ok)
	{
		write(STDERR_FILENO, "minishell: exit: ", 18);
		write(STDERR_FILENO, argv[1], ms_strlen(argv[1]));
		write(STDERR_FILENO,
			": numeric argument required\n", 28);
		ms_set_exit(255);
		return (255);
	}
	if (argv[2])
	{
		write(STDERR_FILENO,
			"minishell: exit: too many arguments\n", 36);
		return (1); // この場合は exit しない
	}
	ms_set_exit((unsigned char)code);
	return ((unsigned char)code);
}

/* ===== ディスパッチ ===== */

int	exec_builtin(char **argv)
{
	if (!argv || !argv[0])
		return (0);
	if (ms_strncmp(argv[0], "echo", 5) == 0)
		return (builtin_echo(argv));
	if (ms_strncmp(argv[0], "cd", 3) == 0)
		return (builtin_cd(argv));
	if (ms_strncmp(argv[0], "pwd", 4) == 0)
		return (builtin_pwd(argv));
	if (ms_strncmp(argv[0], "export", 7) == 0)
		return (builtin_export(argv));
	if (ms_strncmp(argv[0], "unset", 6) == 0)
		return (builtin_unset(argv));
	if (ms_strncmp(argv[0], "env", 4) == 0)
		return (builtin_env(argv));
	if (ms_strncmp(argv[0], "exit", 5) == 0)
		return (builtin_exit(argv));
	return (1);
}
