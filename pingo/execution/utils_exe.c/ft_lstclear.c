/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 20:06:10 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/18 02:58:14 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_exp_clear(t_exp **lst)
{
	t_exp	*current;
	t_exp	*next;

	if (!lst || !*lst)
		return ;
	current = *lst;
	while (current)
	{
		next = current->next;
		if (current->vr)
			free(current->vr);
		if (current->vl)
			free(current->vl);
		free(current);
		current = next;
	}
	*lst = NULL;
}

void	ft_env_clear(t_env **lst)
{
	t_env	*current;
	t_env	*next;

	if (!lst || !*lst)
		return ;
	current = *lst;
	while (current)
	{
		next = current->next;
		if (current->var)
			free(current->var);
		if (current->val)
			free(current->val);
		free(current);
		current = next;
	}
	*lst = NULL;
}
