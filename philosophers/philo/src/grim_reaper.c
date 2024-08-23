/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 22:40:19 by jyap              #+#    #+#             */
/*   Updated: 2024/08/23 10:26:51 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* set_sim_stop_flag:
*	Sets the simulation stop flag to 1 or 0. Only the grim
*	reaper thread can set this flag. If the simulation stop flag is
*	set to 1, that means the simulation has met an end condition.
*/
static void	set_sim_stop_flag(t_table *t, int state)
{
	pthread_mutex_lock(&t->sim_stop_lock);
	t->sim_stop = state;
	pthread_mutex_unlock(&t->sim_stop_lock);
}

/* has_simulation_stopped:
*	Checks whether the simulation is at an end. The stop flag
*	is protected by a mutex lock to allow any thread to check
*	the simulation status without conflict.
*	Returns 1 if the simulation stop flag is set to 1,
*	0 if the flag is set to 0. 
*/
int	has_sim_stopped(t_table *t)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&t->sim_stop_lock);
	if (t->sim_stop == 1)
		i = 1;
	pthread_mutex_unlock(&t->sim_stop_lock);
	return (i);
}

/* kill_philo:
*	Checks if the philosopher must be killed by comparing the
*	time since the philosopher's last meal and the time_to_die parameter.
*	If it is time for the philosopher to die, sets the simulation stop
*	flag and displays the death status.
*	Returns 1 if the philosopher has been killed, 0 if not.
*/
static int	kill_philo(t_philo *philo)
{
	int starve_time;

	starve_time = get_time_difference_in_ms(philo->last_meal);
	if (starve_time >= philo->table->time_to_die)
	{
		set_sim_stop_flag(philo->table, 1);
		write_status(philo, 1, DIED);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (1);
	}
	return (0);
}

/* end_condition_reached:
*	Checks each philosopher to see if one of two end conditions
*	has been reached. Stops the simulation if a philosopher needs
*	to be killed, or if every philosopher has eaten enough.
*	Returns 1 if an end condition has been reached, 0 if not.
*/
static int	end_condition_reached(t_table *t)
{
	int	i;
	int			all_ate_enough;

	all_ate_enough = 1;
	i = 0;
	while (i < t->n_philos)
	{
		pthread_mutex_lock(&t->philos[i]->meal_time_lock);
		if (kill_philo(t->philos[i]))
			return (1);
		if (t->must_eat_count != -1)
			if (t->philos[i]->times_ate < t->must_eat_count)
				all_ate_enough = 0;
		pthread_mutex_unlock(&t->philos[i]->meal_time_lock);
		i++;
	}
	if (t->must_eat_count != -1 && all_ate_enough == 1)
	{
		set_sim_stop_flag(t, 1);
		return (1);
	}
	return (0);
}

/* grim_reaper:
*	The grim reaper thread's routine. Checks if a philosopher must
*	be killed and if all philosophers ate enough. If one of those two
*	end conditions are reached, it stops the simulation.
*/
void	*grim_reaper(void *data)
{
	t_table			*t;

	t = (t_table *)data;
	if (t->must_eat_count == 0)
		return (NULL);
	set_sim_stop_flag(t, 0);
	sim_start_delay(t->start_time);
	while (1)
	{
		if (end_condition_reached(t) == 1)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
