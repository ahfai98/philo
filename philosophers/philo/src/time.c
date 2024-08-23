/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:07:22 by mcombeau          #+#    #+#             */
/*   Updated: 2024/08/23 10:29:01 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* get_time_in_ms:
*	Gets the current time in miliseconds since the Epoch (1970-01-01 00:00:00).
*	Returns the time value.
*/
int	get_time_in_ms(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec + 500) / 1000);
}

struct timeval	get_time_in_timeval(void)
{
	struct timeval	tv;
	
	gettimeofday(&tv, NULL);
	return (tv);
}

int	get_time_difference_in_ms(struct timeval start)
{
	struct timeval	tv;
	int				i;

	gettimeofday(&tv, NULL);
	i = (tv.tv_sec - start.tv_sec) * 1000 + (tv.tv_usec - start.tv_usec + 500) /1000;
	return (i);
}

/* philo_sleep:
*	Pauses the philosopher thread for a certain amount of time in miliseconds.
*	Periodically checks to see if the simulation has ended during the sleep
*	time and cuts the sleep short if it has.
*/
void	philo_sleep(t_table *t, int sleep_time)
{
	int	wake_up;

	wake_up = get_time_in_ms() + sleep_time;
	while (get_time_in_ms() < wake_up)
	{
		if (has_sim_stopped(t))
			break ;
		usleep(100);
	}
}

/* sim_start_delay:
*	Waits for a small delay at the beginning of each threads execution
*	so that all threads start at the same time with the same start time
*	reference. This ensures the grim reaper thread is synchronized with
*	the philosopher threads.
*/
void	sim_start_delay(struct timeval start_time)
{
	while (get_time_difference_in_ms(start_time) < 0)
		continue ;
}
