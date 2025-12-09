/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 00:07:44 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/10 00:07:45 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"

typedef enum e_node_kind
{
	N_COMMAND,
	N_PIPE,
	N_AND,
	N_OR,
}	t_node_kind;

typedef enum e_redir_type
{
	R_IN,
	R_OUT_TRUNC,
	R_OUT_APP,
	R_HERDOC,
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char	**argv;
	t_redir	*redirs;
}	t_cmd;

typedef struct s_ast
{
	t_node_kind		kind;
	struct s_ast	*lhs;
	struct s_ast	*rhs;
	t_cmd			cmd;
}	t_ast;

typedef struct s_parser
{
	const t_token_vec	*tv;
	size_t				pos;
	const char			*err;
}	t_parser;

typedef struct s_str_vec
{
	char	**data;
	size_t	size;
	size_t	cap;
}	t_str_vec;

/* parser.c */
t_ast	*parser_input(t_parser *ps);
t_ast	*parser_and_or(t_parser *ps);
t_ast	*parser_pipeline(t_parser *ps);

/* parser_command.c */
t_ast	*parser_command(t_parser *ps);
int		parser_redir(t_parser *ps, t_redir **out_head);

/* parser_utils.c */
t_token	*peek(t_parser *ps);
t_token	*advance(t_parser *ps);
int		match(t_parser *ps, t_token_kind k);
int		expect(t_parser *ps, t_token_kind k, const char *msg);

/* redir.c */
t_redir	*redir_new(t_redir_type ty, const char *target);
void	redir_push_back(t_redir **head, t_redir *node);
void	redir_free(t_redir *r);

/* str_vec.c */
void	sv_init(t_str_vec *v);
void	sv_push(t_str_vec *v, char *s);
char	**sv_take_null_terminated(t_str_vec *v);
void	free_argv(char **argv);

/* ast_utils.c */
t_ast	*ast_new(t_node_kind k);
void	free_ast(t_ast *n);

/* debug.c */
void	print_ast(t_ast *n, int depth);

#endif
