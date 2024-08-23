/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:30:38 by jyap              #+#    #+#             */
/*   Updated: 2024/08/23 19:30:42 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* free_table:
*	Frees all of the memory allocated by the program.
*	Returns a NULL pointer if there is nothing to free,
*	or when all memory has been freed.
*/
void	*free_table(t_table *t)
{
	int	i;

	if (!t)
		return (NULL);
	if (t->philos != NULL)
	{
		i = 0;
		while (i < t->n_philos)
		{
			if (t->philos[i] != NULL)
			{
				if (t->philos[i]->sem_meal_name)
					free(t->philos[i]->sem_meal_name);
				free(t->philos[i]);
			}
			i++;
		}
		free(t->philos);
	}
	if (t->pids)
		free(t->pids);
	free(t);
	return (NULL);
}

/* sem_error_cleanup:
*	Closes and unlinks all semaphores. Used only during
*	initialization, in case a semaphore fails to be opened.
*	Returns 0 for failure.
*/
int	sem_error_cleanup(t_table *t)
{
	sem_close(t->sem_forks);
	sem_close(t->sem_write);
	sem_close(t->sem_philo_full);
	sem_close(t->sem_philo_dead);
	sem_close(t->sem_stop);
	unlink_global_sems();
	return (error_failure(STR_ERR_SEM, NULL, t));
}

/* table_cleanup:
*	Closes all semaphores and frees any allocated memory.
*	Also waits for the global grim reaper thread to terminate.
*	Returns the provided exit code.
*/
int	table_cleanup(t_table *t, int exit_code)
{
	if (t != NULL)
	{
		pthread_join(t->famine_reaper, NULL);
		pthread_join(t->gluttony_reaper, NULL);
		sem_close(t->sem_forks);
		sem_close(t->sem_write);
		sem_close(t->sem_philo_full);
		sem_close(t->sem_philo_dead);
		sem_close(t->sem_stop);
		unlink_global_sems();
		free_table(t);
	}
	return (exit_code);
}
