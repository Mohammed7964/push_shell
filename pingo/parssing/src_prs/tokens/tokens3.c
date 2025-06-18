/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:09:41 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/18 22:22:29 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_type	get_token_type(char *str, t_gc *gc)
{
	if (gc->count >= 17)
	{
		printf("minishell: maximum here-document count exceeded\n");
		exit(2);
	}
	if (ft_strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_APPEND);
	if (ft_strcmp(str, "||") == 0)
		return (TOKEN_OR);
	if (ft_strcmp(str, "<<") == 0)
		return (gc->count++, TOKEN_HEREDOC);
	if (ft_strcmp(str, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	if (ft_strcmp(str, ">") == 0)
		return (TOKEN_REDIRECT_OUT);
	if (ft_strcmp(str, "(") == 0)
		return (TOKEN_OPEN_PAREN);
	if (ft_strcmp(str, " ") == 0)
		return (TOKEN_SPACE);
	return (TOKEN_WORD);
}

int	tokenize_next_token(t_gc *gc, char *input, t_token **head, int i)
{
	int		start;
	char	*tmp;

	if (ft_issspace(input[i]))
	{
		start = i;
		while (ft_issspace(input[i]))
			i++;
		tmp = gc_substr(gc, input, start, i - start);
		append_token(head, create_token(gc, tmp, TOKEN_SPACE));
	}
	else if (redir_pipe(input[i]))
		i = handle_redir(gc, input, head, i);
	else
		i = add_word5(gc, input, head, i);
	return (i);
}

void	mark_file_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		if (tokens->type == TOKEN_REDIRECT_IN
			|| tokens->type == TOKEN_REDIRECT_OUT
			|| tokens->type == TOKEN_APPEND
			|| tokens->type == TOKEN_HEREDOC)
		{
			next = tokens->next;
			while (next && next->type == TOKEN_SPACE)
				next = next->next;
			if (next && next->type == TOKEN_WORD)
			{
				next->type = TOKEN_FILE;
			}
		}
		tokens = tokens->next;
	}
}

int	handle_redir(t_gc *gc, char *input, t_token **head, int i)
{
	int		start;
	char	*tmp;
	t_token	*new_token;

	start = i;
	if ((input[i] == '>' || input[i] == '<') && input[i + 1] == input[i])
		i += 2;
	else
		i++;
	tmp = gc_substr(gc, input, start, i - start);
	new_token = create_token(gc, tmp, get_token_type(tmp, gc));
	append_token(head, new_token);
	return (i);
}

int	add_word5(t_gc *gc, char *input, t_token **head, int i)
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
