/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 10:58:43 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/17 14:00:38 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_escaped_quote(char *str, int i)
{
	return (str[i] == '\\' && (str[i + 1] == '\'' || str[i + 1] == '"'
			|| str[i + 1] == '\\'));
}

int	handle_quoted_content(char *str, int i, char *res, int *j)
{
	char	quote;

	quote = str[i++];
	while (str[i] && str[i] != quote)
		res[(*j)++] = str[i++];
	if (str[i] == quote)
		i++;
	return (i);
}

void	remove_quotes_array(t_gc *gc, char **arr)
{
	int		i;
	char	*stripped;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		stripped = strip_quotes(gc, arr[i]);
		arr[i] = stripped;
		i++;
	}
}

void	remove_quotes_cmd(t_gc *gc, t_cmd *cmd)
{
	while (cmd)
	{
		remove_quotes_array(gc, cmd->cmd);
		remove_quotes_array(gc, cmd->args);
		remove_quotes_array(gc, cmd->files);
		remove_quotes_array(gc, cmd->redirection);
		cmd = cmd->next;
	}
}

void	handle_quotes(t_ctx *ctx, char c)
{
	if (c == '\'' && !(*ctx->d))
	{
		if (*ctx->s)
			*ctx->s = 0;
		else
			*ctx->s = 1;
	}
	else if (c == '"' && !(*ctx->s))
	{
		if (*ctx->d)
			*ctx->d = 0;
		else
			*ctx->d = 1;
	}
}
