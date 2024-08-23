/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 22:40:19 by jyap              #+#    #+#             */
/*   Updated: 2024/08/22 23:02:46 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*free_table(t_table *t)
{
	int	i;

	if (!t)
		return (NULL);
	if (t->fork_locks != NULL)
		free(t->fork_locks);
	if (t->philos != NULL)
	{
		i = 0;
		while (i < t->n_philos)
		{
			if (t->philos[i] != NULL)
				free(t->philos[i]);
			i++;
		}
		free(t->philos);
	}
	free(t);
	return (NULL);
}

void	destroy_mutexes(t_table *t)
{
	int	i;

	i = 0;
	while (i < t->n_philos)
	{
		pthread_mutex_destroy(&t->fork_locks[i]);
		pthread_mutex_destroy(&t->philos[i]->meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&t->write_lock);
	pthread_mutex_destroy(&t->sim_stop_lock);
}


int	msg(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_no);
}

int	error_failure(char *str, char *details, t_table *t)
{
	if (t != NULL)
		free_table(t);
	return (msg(str, details, 0));
}

void	*error_null(char *str, char *details, t_table *t)
{
	if (t != NULL)
		free_table(t);
	msg(str, details, EXIT_FAILURE);
	return (NULL);
}
