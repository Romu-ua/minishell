#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

typedef enum
{
	N_COMMAND,	// argv + redir
	N_PIPE,		// |
	N_AND,		// &&
	N_OR,		// ||
}	e_node_kind;

typedef enum
{
	R_IN,			// <
	R_OUT_TRUNC,	// >
	R_OUT_APP,		// >>
	R_HERDOC,		// <<
}	e_redir_type;

typedef struct s_redir
{
	e_redir_type	type;
	char			*target;	// ファイル名 or ヒアドキュメントのデリミタ
	struct s_redir	*next;		// リダイレクトは出現順に単方向リスト
}	t_redir;

typedef struct s_cmd
{
	char		**argv;
	t_redir		*redirs;
}	t_cmd;

typedef struct s_ast
{
	e_node_kind		kind;
	struct s_ast	*lhs;
	struct s_ast	*rhs;
	t_cmd			cmd;
}	t_ast;

typedef struct s_str_vec
{
	char	**data;
	size_t	size;
	size_t	cap;
}	t_str_vec;

typedef struct s_parser
{
	const t_token_vec	*tv;
	size_t				pos;
	const char			*err;
}	t_parser;


// 文字配列 ユーティリティ

static void	sv_init(t_str_vec *v)
{
	v->data = NULL;
	v->cap = 0;
	v->size = 0;
}

// 配列に文字列を入れる
static void	sv_push(t_str_vec *v, char *s)
{
	char	**nd;
	size_t	ncap;

	if (v->size == v->cap)
	{
		if (v->cap)
			ncap = v->cap * 2;
		else
			ncap = 8;
		nd = (char **)realloc(v->data, ncap * sizeof(char *));
		if (!nd)
		{
			perror("realloc");
			exit(1);
		}
		v->cap = ncap;
		v->data = nd;
	}
	v->data[v->size++] = s;
}

// NULL文字列を最後に入れ、文字列の配列を返す
static char	**sv_take_null_terminated(t_str_vec *v)
{
	char	**r;

	sv_push(v, NULL);
	r = v->data;
	v->data = NULL;
	v->cap = 0;
	v->size = 0;
	return (r);
}

// freeする
// svから所有権は離れているので引数はchar **になる
static void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

// Redir ユーティリティ
// 単方向リストのユーティリティとほぼ同じ。

// 新しいリストの先頭を作成する
static t_redir	*redir_new(e_redir_type ty, const char *target)
{
	t_redir	*r;

	r =  (t_redir *)calloc(1, sizeof(t_redir));
	if (!r)
	{
		perror("calloc");
		exit(1);
	}
	r->type = ty;
	r->target = strdup(target ? target : "");
	return (r);
}

// 新しいリストを末尾につける
static void	redir_push_back(t_redir **head, t_redir *node)
{
	t_redir	*cur;
	
	if (!*head)
	{
		*head = node;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

// freeする
static void	redir_free(t_redir *r)
{
	t_redir	*nx;

	while (r)
	{
		nx = r->next;
		free(r->target);
		free(r);
		r = nx;
	}
}

// ast ユーティリティ

// 新しくノードを作る
static t_ast	*ast_new(e_node_kind k)
{
	t_ast	*n;

	n = (t_ast *)calloc(1, sizeof(t_ast));
	if (!n)
	{
		perror("calloc");
		exit(1);
	}
	n->kind = k;
	return (n);
}

// freeする
static void	free_ast(t_ast *n)
{
	if (!n)
		return ;
	free_ast(n->lhs);
	free_ast(n->rhs);
	if (n->kind == N_COMMAND)
	{
		free_argv(n->cmd.argv);
		redir_free(n->cmd.redirs);
	}
	free(n);
}

// パーサー ユーティリティ

// 現在見ている位置のt_token *を返す
static t_token	*peek(t_parser *ps)
{
	return (&ps->tv->data[ps->pos]);
}

// 現在見ている位置のToken *を返して、実際にposを進める
static t_token	*advance(t_parser *ps)
{
	return (&ps->tv->data[ps->pos++]);
}

// token_kindと一致していたら現在見ている位置(pos)を進めて、1を返す
static int	match(t_parser *ps, e_token_kind k)
{
	if (peek(ps)->kind == k)
	{
		ps->pos++;
		return (1);
	}
	return (0);
}

// 現在のtoken_kindがkであることを確認して、そうでないときは、psのerrにmsgを入れる
// matchを使っているのでposは進んでいる。
static int	expect(t_parser *ps, e_token_kind k, const char *msg)
{
	if (match(ps, k))
		return (1);
	ps->err = msg;
	return (0);
}

// redir := '<' WORD | '>' WORD | '>>' WORD | '<<' WORD 
// 葉のcmdの中のredirを作成している。
// リダイレクトではない時は0
// リダイレクトの右側にwordがない時はerrorで-1
// 成功時は1;
static int	parser_redir(t_parser *ps, t_redir **out_head)
{
	e_token_kind	k;
	e_redir_type	rt;
	const t_token	*t;

	k = peek(ps)->kind;
	if (k == T_LT)
		rt = R_IN;
	else if (k == T_GT)
		rt = R_OUT_TRUNC;
	else if (k == T_DGT)
		rt = R_OUT_APP;
	else if (k == T_DLT)
		rt = R_HERDOC;
	else
		return (0);
	advance(ps); // 現在見ているtokenを消費
	if (!expect(ps, T_WORD, "redir expects WORD on the right"))
		return (-1);
	t = &ps->tv->data[ps->pos-1]; // expectで一つ進めているから、-1が必要。
	redir_push_back(out_head, redir_new(rt, t->lexeme));
	return (1);
}

// comand := (redir)* WORD (WORD | redir)* | (redir)+
// 最小単位の葉を作成しているのはここになる。
// cmdのargvをこちらで作成して、redirはparser_redirに任せている。
static t_ast	*parser_command(t_parser *ps)
{
	t_str_vec		argv;
	t_redir			*rlist;
	int				progress;
	const t_token	*t;
	t_ast			*n;

	rlist = NULL;
	sv_init(&argv);
	// 先頭のリダイレクトを許す
	while (1)
	{
		progress = parser_redir(ps, &rlist);
		if (progress != 1)
			break ;
	}
	if (progress == -1)
	{
		ps->err = "invalid redirection";
		return (NULL);
	}
	// echo -nに分かれた文字列を一つの配列にすることでN_COMMANDにしている
	// 実行可能なコマンドとオプションの間にリダイレクトがあっても機能する
	if (peek(ps)->kind == T_WORD)
	{
		while (peek(ps)->kind == T_WORD)
		{
			t = advance(ps);
			sv_push(&argv, strdup(t->lexeme));

			while (1)
			{
				progress = parser_redir(ps, &rlist);
				if (progress != 1)
					break ;
			}
			if (progress == -1)
			{
				ps->err = "invalid redirection";
				return (NULL);
			}
		}
	}
	else if (!rlist)
	{
		ps->err = "expected command or redirection"; // 何もないはエラー
		for (size_t i = 0; i < argv.size; i++)
			free(argv.data[i]);
		free(argv.data);
		redir_free(rlist);
		return (NULL);
	}
	n = ast_new(N_COMMAND);
	n->cmd.argv = sv_take_null_terminated(&argv);
	n->cmd.redirs = rlist;
	return (n);
}

// pipeline := command ( '|' comand)*
static t_ast	*parser_pipeline(t_parser *ps)
{

	t_ast	*node;
	t_ast	*rhs;	// PIPEがある時、その右側
	t_ast	*p;		// PIPEで繋がっている時は木として返すのでそのroot

	node = parser_command(ps);

	if (!node)
		return (NULL);
	while (peek(ps)->kind == T_PIPE)
	{
		advance(ps);	// 現在見ているtokenを消費

		rhs = parser_command(ps);
		if (!rhs)
		{
			free_ast(node);
			return (NULL);
		}
		p = ast_new(N_PIPE);
		p->rhs = rhs;
		p->lhs = node;
		node = p;
	}
	return (node);
}

// and_or := pipeline (('&&' || '||') pipeline)*
static t_ast	*parser_and_or(t_parser *ps)
{
	// paser_pipelineから渡されるのは木である可能性もあるが
	// 木全体をnodeとして考えている。
	t_ast			*node;
	t_ast			*rhs;
	t_ast			*p;
	e_node_kind		nk;

	node = parser_pipeline(ps);
	if (!node)
		return (NULL);
	while (peek(ps)->kind == T_OR_IF || peek(ps)->kind == T_AND_IF)
	{
		if (peek(ps)->kind == T_AND_IF)
			nk = N_AND;
		else
			nk = N_OR;
		advance(ps);
		rhs = parser_pipeline(ps);
		if (!rhs)
		{
			free_ast(node);
			return (NULL);
		}
		p = ast_new(nk);
		p->rhs = rhs;
		p->lhs = node;
		node = p;
	}
	return (node);
}

// input := and_or EOF
// and_orに渡して、次がEOF以外ならエラー
static t_ast	*parser_input(t_parser *ps)
{
	t_ast	*root = parser_and_or(ps);
	if (!root)
		return (NULL);
	if (peek(ps)->kind != T_EOF)
	{
		ps->err = "unexpected token after experssion";
		free_ast(root);
		return (NULL);
	}
	return (root);
}



// debug

static char	*rname(e_redir_type t)
{
	switch (t)
	{
		case R_IN:
			return ("<");
			break ;
		case R_OUT_TRUNC:
			return (">");
			break ;
		case R_OUT_APP:
			return (">>");
			break ;
		case R_HERDOC:
			return ("<<");
			break ;
	}
	return ("?");
}

// 行きがけ順で表示
// 実際の実行は帰りがけ順
static void	print_ast(t_ast *n, int depth)
{
	if (!n)
		return ;
	for (int i = 0; i < depth; i++)
		putchar(' ');
	switch (n->kind)
	{
		case N_PIPE:
			puts("(PIPE)");
			print_ast(n->lhs, depth + 2);
			print_ast(n->rhs, depth + 2);
			break ;
		case N_AND:
			puts("(&&)");
			print_ast(n->lhs, depth + 2);
			print_ast(n->rhs, depth + 2);
			break ;
		case N_OR:
			puts("(||)");
			print_ast(n->lhs, depth + 2);
			print_ast(n->rhs, depth + 2);
			break ;
		case N_COMMAND:
			printf("(CMD");
			if (n->cmd.argv)
			{
				for (size_t i = 0; n->cmd.argv[i]; i++)
					printf(" \"%s\"", n->cmd.argv[i]);
			}
			printf(")\n");
			// ASTとしてはこの表示が正しいが、実際はcmdにredirもある。
			for (t_redir *r = n->cmd.redirs; r; r = r->next)
			{
				for (int i = 0; i < depth + 2; i++)
					putchar(' ');
				printf("(REDIR %s \"%s\")\n", rname(r->type), r->target);
			}
			break;
	}
}


int	main(void)
{
	char		buf[4096];
	t_token_vec	tv;
	int			ok;
	int			haserr;
	int			i;
	t_parser	ps;
	t_ast		*root;

	printf("parse> ");
	while (fgets(buf, sizeof(buf), stdin))
	{
		if (lex_line(buf, &tv) == 0)
			ok = 1;
		else
			ok = 0;
		haserr = 0;
		i = 0;
		while (i < tv.size)
		{
			if (tv.data[i].kind == T_ERROR)
				haserr = 1;
			i++;
		}
		if (!ok || haserr)
		{
			puts("[lexer error]");
			tv_free(&tv);
			printf("parser> ");
			continue ;
		}
		
		ps = (t_parser){ .tv=&tv, .pos = 0, .err = NULL };
		root = parser_input(&ps);
		if (!root)
			fprintf(stderr, "[parse error] %s\n", ps.err ? ps.err : "invalid syntax");
		else
		{
			puts("== AST ==");
			print_ast(root, 0);
			free_ast(root);
		}
		tv_free(&tv);
		printf("parser> ");
	}
}
