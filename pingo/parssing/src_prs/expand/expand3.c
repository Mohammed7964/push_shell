/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:44:14 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/17 12:38:09 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*get_var_name(t_gc *gc, char **src)
{
	char	var[256];
	int		i;

	i = 0;
	if (**src == '?')
	{
		(*src)++;
		return (gc_strdup(gc, "?"));
	}
	while (**src && (ft_isalnum(**src) || **src == '_') && i < 255)
		var[i++] = *((*src)++);
	var[i] = '\0';
	if (!*var)
		return (NULL);
	return (gc_strdup(gc, var));
}

void	change_quotes_to_chars(char *var)
{
	int	i;

	i = 0;
	if (!var)
		return ;
	while (var[i])
	{
		if (var[i] == '\'')
			var[i] = '\x01';
		else if (var[i] == '"')
			var[i] = '\x02';
		i++;
	}
}

char	*get_var_value(t_gc *gc, char **src, t_env *env, t_cmd *cmd)
{
	char	*var;
	char	*val;

	var = get_var_name(gc, src);
	if (!var)
		return (NULL);
	if (ft_strcmp(var, "?") == 0)
	{
		return (gc_itoa(gc, cmd->ex_status));
	}
	val = find_env(env, var);
	change_quotes_to_chars(val);
	return (gc_strdup(gc, val));
}

void	append_dollars(t_gc *gc, t_dynstr *ds, int count)
{
	while (count--)
		dyn_str_append(gc, ds, "$", 1);
}

void	return_characters(char **cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!cmd)
		return ;
	while (cmd[i])
	{
		j = 0;
		while (cmd[i][j])
		{
			if (cmd[i][j] == '\x01')
				cmd[i][j] = '\'';
			else if (cmd[i][j] == '\x02')
				cmd[i][j] = '"';
			j++;
		}
		i++;
	}
}
