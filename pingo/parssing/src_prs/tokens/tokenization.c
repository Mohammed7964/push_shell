/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:14:49 by mel-badd          #+#    #+#             */
/*   Updated: 2025/05/23 23:42:28 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	add_word(t_gc *gc, char *input, t_token **head, int i)
{
	int		start;
	char	*tmp;
	char	quote;

	start = i;
	while (input[i] && !ft_issspace(input[i]) && !redir_pipe(input[i]))
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i++];
			while (input[i] && input[i] != quote)
				i++;
			if (!input[i])
				return (gc->single = 3, ft_putstr_fd("minishell: syntax error: \
unclosed quote\n", 2), -1);
			i++;
		}
		else
			i++;
	}
	tmp = gc_substr(gc, input, start, i - start);
	if (!tmp)
		return (-1);
	return (append_token(head, create_token(gc, tmp, TOKEN_WORD)), i);
}

int	check_ambiguous_redirection(t_ctx *ctx, t_token *tokens)
{
	t_token	*cur;
	t_token	*target;

	cur = tokens;
	while (cur)
	{
		if (cur->type == TOKEN_REDIRECT_IN
			|| cur->type == TOKEN_REDIRECT_OUT
			|| cur->type == TOKEN_APPEND
			|| cur->type == TOKEN_HEREDOC)
		{
			target = cur->next;
			while (target && target->type == TOKEN_SPACE)
				target = target->next;
			if (target && target->type == TOKEN_FILE)
				if (is_ambiguous(ctx, target))
					return (0);
		}
		cur = cur->next;
	}
	return (1);
}

int	count_pipes(t_token *tokens, t_ctx *ctx)
{
	t_token	*cur;
	int		count;

	cur = tokens;
	count = 0;
	while (cur)
	{
		if (cur->type == TOKEN_PIPE)
			count++;
		cur = cur->next;
	}
	ctx->numcmd = count + 1;
	return (count);
}

t_token	*prepare_tokens(char *input, t_ctx *ctx, t_gc *gc, t_env *env)
{
	t_token	*tokens;
	int		i;

	i = 0;
	ctx->env = &env;
	if (!input)
		return (NULL);
	tokens = tokenize2(gc, input);
	if (gc->single == 3)
		ctx->cmd->ex_status = 258;
	if (!tokens)
		return (NULL);
	i = count_pipes(tokens, ctx);
	if (i > 0)
		ctx->cmd->ex_status = 0;
	mark_file_tokens(tokens);
	if (!error(gc, tokens, ctx))
		return (NULL);
	ctx->gc = gc;
	if (!check_ambiguous_redirection(ctx, tokens))
		return (ctx->cmd->ex_status = 1, NULL);
	tokens = expand(ctx, tokens, ctx->cmd);
	if (!tokens)
		return (NULL);
	return (tokens);
}

t_cmd	*parse(t_gc *gc, t_env *env, t_heredoc *heredoc, t_cmd **cmd_1)
{
	t_token			*tokens;
	char			*input;
	char			*copy;
	t_cmd			*new_cmd;
	t_ctx			ctx;

	ctx.heredoc = heredoc;
	ctx.cmd = *cmd_1;
	input = read_input("minishell$ ", ctx);
	if (!input)
		return (NULL);
	(ex_staus_ctr_c(ctx), copy = gc_malloc(gc, ft_strlen(input) + 1));
	if (!copy)
		return (NULL);
	(ft_strcpy(copy, input), free(input));
	tokens = prepare_tokens(copy, &ctx, gc, env);
	if (!tokens)
		return (gc_free_all(gc), NULL);
	new_cmd = joining2(gc, tokens);
	if (!new_cmd)
		return (gc_free_all(gc), NULL);
	new_cmd->ex_status = (*cmd_1)->ex_status;
	remove_quotes_cmd(gc, new_cmd);
	return_characters(new_cmd->cmd);
	return (new_cmd);
}
