/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 11:46:06 by mcombeau          #+#    #+#             */
/*   Updated: 2024/08/23 19:52:20 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* start_simulation:
*	Launches the simulation by creating a grim reaper thread as well as
*	one thread for each philosopher.
*	Returns 1 if the simulation was successfully started, 0 if there
*	was an error. 
*/
static int	start_sim(t_table *t)
{
	int	i;

	t->start_time = get_time_in_timeval();
	t->start_time.tv_usec += t->n_philos * 10000;
	if (t->start_time.tv_usec >= 1000000)
	{
		t->start_time.tv_sec++;
		t->start_time.tv_usec -= 1000000;
	}
	i = -1;
	while (++i < t->n_philos)
	{
		if (pthread_create(&t->philos[i]->thread, NULL,
				&philosopher, t->philos[i]) != 0)
		{
			destroy_mutexes(t);
			return (error_failure(STR_ERR_THREAD, NULL, t));
		}
	}
	if (t->n_philos > 1)
	{
		if (pthread_create(&t->grim_reaper, NULL,
				&grim_reaper, t) != 0)
		{
			destroy_mutexes(t);
			return (error_failure(STR_ERR_THREAD, NULL, t));
		}
	}
	return (1);
}

/* stop_simulation:
*	Waits for all threads to be joined then destroys mutexes and frees
*	allocated memory.
*/
static void	stop_sim(t_table	*t)
{
	int	i;

	i = 0;
	while (i < t->n_philos)
	{
		pthread_join(t->philos[i]->thread, NULL);
		i++;
	}
	if (t->n_philos > 1)
		pthread_join(t->grim_reaper, NULL);
	destroy_mutexes(t);
	free_table(t);
}

int	main(int ac, char **av)
{
	t_table	*t;

	t = NULL;
	if (ac < 5 || ac > 6)
		return (msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!is_valid_input(ac, av))
		return (EXIT_FAILURE);
	t = init_table(ac, av, 1);
	if (!t)
		return (EXIT_FAILURE);
	if (!start_sim(t))
		return (EXIT_FAILURE);
	stop_sim(t);
	return (EXIT_SUCCESS);
}
