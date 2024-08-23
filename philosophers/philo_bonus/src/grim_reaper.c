/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:31:06 by jyap              #+#    #+#             */
/*   Updated: 2024/08/23 19:31:07 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "philo_bonus.h"

/* kill_all_philos:
*	Sends the kill signal to all philosopher processes. Used to send the
*	simulation if a philosopher has died or if a philosopher process has
*	encountered a fatal error.
*	Returns the given exit code.
*/
int	kill_all_philos(t_table *t, int exit_code)
{
	int	i;

	i = 0;
	while (i < t->n_philos)
	{
		kill(t->pids[i], SIGKILL);
		i++;
	}
	return (exit_code);
}

/* global_gluttony_reaper:
*	Kills all philosophers if each one has eaten enough. Each philosopher
*	process decrements the sem_philo_full semaphore. This thread registers
*	those decrementations to count how many philosophers have eaten enough.
*	If all philos have eaten enough, kills all philos to end simulation.
*/
void	*global_gluttony_reaper(void *data)
{
	t_table	*t;

	t = (t_table *)data;
	if (t->must_eat_count < 0 || t->time_to_die == 0
		|| t->n_philos == 1)
		return (NULL);
	sim_start_delay(t->start_time);
	while (t->philo_full_count < t->n_philos)
	{
		if (has_sim_stopped(t) == 1)
			return (NULL);
		sem_wait(t->sem_philo_full);
		if (has_sim_stopped(t) == 0)
			t->philo_full_count += 1;
		else
			return (NULL);
	}
	sem_wait(t->sem_stop);
	t->stop_sim = 1;
	kill_all_philos(t, EXIT_SUCCESS);
	sem_post(t->sem_philo_dead);
	sem_post(t->sem_stop);
	return (NULL);
}

/* global_famine_reaper:
*	Kills all philosophers if one has died. Each philosopher
*	process decrements the sem_philo_dead semaphore upon philo death.
*	This thread registers the first decrementation and kills all philos
*	immediately.
*/
void	*global_famine_reaper(void *data)
{
	t_table	*t;

	t = (t_table *)data;
	if (t->n_philos == 1)
		return (NULL);
	sim_start_delay(t->start_time);
	if (has_sim_stopped(t) == 1)
		return (NULL);
	sem_wait(t->sem_philo_dead);
	if (has_sim_stopped(t) == 1)
		return (NULL);
	sem_wait(t->sem_stop);
	t->stop_sim = 1;
	kill_all_philos(t, EXIT_SUCCESS);
	sem_post(t->sem_philo_full);
	sem_post(t->sem_stop);
	return (NULL);
}

/* end_condition_reached:
*	Checks this philosopher to see if one of two end conditions
*	has been reached. If the philosopher has died, decrements a semaphore
*	that will trigger "famine reaper" to kill all philos.
*	If the philosopher has eaten enough, decrements a semaphore that
*	will trigger "gluttony_reaper" to increase philo_full_count.
*	Returns 0 if the philosopher is alive.
*/
static int	end_condition_reached(t_table *t, t_philo *philo)
{
	sem_wait(philo->sem_meal);
	if (get_time_in_ms() - philo->last_meal >= t->time_to_die)
	{
		write_status(philo, 1, DIED);
		sem_post(t->this_philo->sem_philo_dead);
		sem_post(philo->sem_meal);
		return (1);
	}
	if (t->must_eat_count != -1 && philo->ate_enough == 0
		&& philo->times_ate >= (int)t->must_eat_count)
	{
		sem_post(philo->sem_philo_full);
		philo->ate_enough = 1;
	}
	sem_post(philo->sem_meal);
	return (0);
}

/* personal_grim_reaper:
*	The grim reaper thread's routine. Checks if this philosopher must
*	be killed and if he ate enough.
*/
void	*personal_grim_reaper(void *data)
{
	t_table			*t;

	t = (t_table *)data;
	if (t->must_eat_count == 0)
		return (NULL);
	sem_wait(t->this_philo->sem_philo_dead);
	sem_wait(t->this_philo->sem_philo_full);
	sim_start_delay(t->start_time);
	while (!end_condition_reached(t, t->this_philo))
	{
		usleep(1000);
		continue ;
	}
	return (NULL);
}
