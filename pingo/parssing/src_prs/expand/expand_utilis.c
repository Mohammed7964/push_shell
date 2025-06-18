/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utilis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 05:20:20 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/17 18:30:35 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	count_dolar(char **src)
{
	int	count;

	count = 0;
	while (**src == '$')
	{
		count++;
		(*src)++;
	}
	return (count);
}

static void	append_dollar_pairs(t_gc *gc, t_dynstr *ds, int count)
{
	int	pairs;

	if (count < 2)
		return ;
	pairs = count / 2;
	while (pairs-- > 0)
		dyn_str_append(gc, ds, "$", 1);
}

void	handle_dollar(t_ctx *ctx, char **src, t_dynstr *ds, t_cmd *cmd)
{
	int		count;
	char	*val;

	count = count_dolar(src);
	if (count == 1 && ((**src == '"' || **src == '\'') && !(*ctx->d)))
		return ;
	append_dollar_pairs(ctx->gc, ds, count);
	if (count % 2 == 1)
	{
		if (**src && (ft_isalpha(**src) || **src == '_' || **src == '?'))
		{
			val = get_var_value(ctx->gc, src, *ctx->env, cmd);
			if (val)
				dyn_str_append(ctx->gc, ds, val, ft_strlen(val));
		}
		else
			dyn_str_append(ctx->gc, ds, "$", 1);
	}
}

void	ex_staus_ctr_c(t_ctx ctx)
{
	if (g_sig == 1)
	{
		g_sig = 0;
		ctx.cmd->ex_status = 1;
	}
}

int	is_ambiguous(t_ctx *ctx, t_token *target)
{
	char	*expanded;

	expanded = NULL;
	if (target->value[0] == '$')
	{
		expand_heredoc(ctx, target);
		if (!expanded || ft_strcmp(expanded, "") == 0)
		{
			printf("minishell: %s: ambiguous redirect\n", target->value);
			return (1);
		}
	}
	return (0);
}
