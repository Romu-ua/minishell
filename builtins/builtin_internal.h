/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:01:53 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:01:53 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_INTERNAL_H
# define BUILTIN_INTERNAL_H

# include "exec.h"

typedef int	(*t_bi_fn)(char **argv);

typedef struct s_bi_ent
{
	const char	*name;
	t_bi_fn		fn;
}	t_bi_ent;

struct s_cd
{
	char		oldpwd[4096];
	char		newpwd[4096];
	const char	*path;
};

int		is_builtin(const char *cmd);
int		exec_builtin(char **argv);
int		bi_echo(char **argv);
int		bi_cd(char **argv);
int		bi_pwd(char **argv);
int		bi_export(char **argv);
int		bi_unset(char **argv);
int		bi_env(char **argv);
int		bi_exit(char **argv);
void	bi_print_export(void);
int		bi_is_identifier(const char *s);
t_env	**bi_export_array(int *out_n);
void	bi_export_sort(t_env **arr, int n);
long	bi_exit_atol(const char *s, int *ok);
void	setup_signal_exec(void);

#endif
