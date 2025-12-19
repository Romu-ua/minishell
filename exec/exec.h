/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:05:52 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:05:53 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include "parser.h"
# include "main.h"
# include "ms_signal.h"
# include "ms_utils.h"

# define MS_STDERR 2

typedef struct s_env
{
	char			*name;
	char			*value;
	int				exported;
	struct s_env	*next;
}	t_env;

int			env_init(char **envp);
const char	*env_get(const char *name);
int			env_set(const char *name, const char *value, int exported);
void		env_unset(const char *name);
char		**env_to_envp_exec(void);
void		env_free_envp(char **envp);
t_env		*env_list(void);
void		env_clear(void);
int			expand_cmd(t_cmd *cmd);
int			expand_ast(t_ast *node);
char		*expand_heredoc_line(const char *src);
void		ms_register_ast(t_ast *root);
void		ms_cleanup_and_exit(int status) __attribute__((noreturn));
void		fatal(const char *msg) __attribute__((noreturn));
int			is_empty_command(t_cmd *cmd);
int			exec_ast(t_ast *node);
void		exec_command_child(t_cmd *cmd);
int			exec_single_command(t_cmd *cmd);
int			exec_pipeline_node(t_ast *pipe_root);
int			apply_redirs(t_redir *r);
int			is_builtin(const char *cmd);
int			exec_builtin(char **argv);
void		ms_set_exit(int status);
int			ms_should_exit(void);
void		ms_reset_exit(void);

#endif
