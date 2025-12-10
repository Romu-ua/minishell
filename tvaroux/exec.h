/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:37:07 by tomvaroux         #+#    #+#             */
/*   Updated: 2025/12/10 17:25:31 by tomvaroux        ###   ########.fr       */
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
# include <ctype.h>

# include "../parser.h"
# include "../main.h"

# include <signal.h>

// extern volatile sig_atomic_t	g_exit_status;	//	これでできるのかな？

// extern char	**environ;

// typedef enum e_node_kind
// {
// 	N_COMMAND,	// argv + redir
// 	N_PIPE,		// |
// 	N_AND,		// &&
// 	N_OR,		// ||
// }	t_node_kind;

// typedef enum e_redir_type
// {
// 	R_IN,			// <
// 	R_OUT_TRUNC,	// >
// 	R_OUT_APP,		// >>
// 	R_HERDOC,		// <<
// }	t_redir_type;

// typedef struct s_redir
// {
// 	t_redir_type	type;
// 	char			*target;
// 	struct s_redir	*next;
// }	t_redir;

// typedef struct s_cmd
// {
// 	char		**argv;
// 	t_redir		*redirs;
// }	t_cmd;

// typedef struct s_ast
// {
// 	t_node_kind		kind;
// 	struct s_ast	*lhs;
// 	struct s_ast	*rhs;
// 	t_cmd			cmd;
// }	t_ast;

/* ===== 環境変数テーブル ===== */

typedef struct s_env
{
	char			*name;
	char			*value;
	int				exported;
	struct s_env	*next;
}	t_env;

// typedef struct s_shell
// {
// 	int		last_status;	/* $? */
// 	t_env	*env;			/* 環境変数リスト */
// }	t_shell;

// extern t_shell	g_shell;




/* env.c */

void		env_init(char **envp);
const char	*env_get(const char *name);
int			env_set(const char *name, const char *value, int exported);
void		env_unset(const char *name);
char		**env_to_envp(void);
void		env_free_envp(char **envp);
void		env_clear(void);

/* expand.c */

// int			expand_word(const char *src, char **out);
// int			expand_cmd(t_cmd *cmd);
int			expand_ast(t_ast *node);
char		*expand_heredoc_line(const char *src);

/* exec_root.c / command.c / pipeline.c / redirect.c */

void		fatal(const char *msg);
int			is_empty_command(t_cmd *cmd);

// int			is_builtin(const char *cmd);
// int			exec_builtin(char **argv);

int			exec_ast(t_ast *node);
void		exec_command_child(t_cmd *cmd);
int			exec_single_command(t_cmd *cmd);
int			exec_pipeline_node(t_ast *pipe_root);
int			apply_redirs(t_redir *r);


/* builtin.c / exit_state.c exit状態管理 */
int			is_builtin(const char *cmd);
int			exec_builtin(char **argv);

void		ms_set_exit(int status);
int			ms_should_exit(void);
int			ms_exit_status(void);
void		ms_reset_exit(void);


/*
 * free_ast は parser.c 側で実装して、ここに extern 宣言してもOK。
 * すでにあるなら static を外して、このプロトタイプに合わせてね。
 */
/* void		free_ast(t_ast *n); */

#endif
