/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:31:43 by jyap              #+#    #+#             */
/*   Updated: 2024/08/23 19:48:43 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "philo_bonus.h"

/* eat_sleep_routine:
*	When a philosopher is ready to eat, he will try to acquire fork semaphores.
*	Then the philosopher will eat for a certain amount of time. The time of
*	the last meal is recorded at the beginning of the meal, not at the end,
*	as per the subject's requirements.
*/
static void	routine(t_philo *philo)
{
	grab_fork(philo);
	grab_fork(philo);
	write_status(philo, 0, EATING);
	sem_wait(philo->sem_meal);
	philo->last_meal = get_time_in_ms();
	sem_post(philo->sem_meal);
	philo_sleep(philo->table->time_to_eat);
	write_status(philo, 0, SLEEPING);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
	sem_wait(philo->sem_meal);
	philo->nb_forks_held -= 2;
	philo->times_ate += 1;
	sem_post(philo->sem_meal);
	philo_sleep(philo->table->time_to_sleep);
}

/* think_routine:
*	Once a philosopher is done sleeping, he will think for a certain
*	amount of time before starting to eat again.
*	The time_to_think is calculated depending on how long it has been
*	since the philosopher's last meal, the time_to_eat and the time_to_die
*	to determine when the philosopher will be hungry again.
*	This helps stagger philosopher's eating routines to avoid forks being
*	needlessly monopolized by one philosopher to the detriment of others.
*/
static void	think_routine(t_philo *philo, int silent)
{
	int	time_to_think;

	sem_wait(philo->sem_meal);
	time_to_think = (philo->table->time_to_die
			- (get_time_in_ms() - philo->last_meal)
			- philo->table->time_to_eat) / 2;
	sem_post(philo->sem_meal);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == 1)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == 0)
		write_status(philo, 0, THINKING);
	philo_sleep(time_to_think);
}

/* lone_philo_routine:
*	This routine is invoked when there is only a single philosopher.
*	A single philosopher only has one fork, and so cannot eat. The
*	philosopher will pick up that fork, wait as long as time_to_die and die.
*	This is a separate routine to make sure that the process does not get
*	stuck waiting for a fork or a writing semaphore that will never unlock.
*/
static void	lone_philo_routine(t_philo *philo)
{
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->n_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		exit(CHILD_EXIT_ERR_SEM);
	sem_wait(philo->sem_philo_full);
	sim_start_delay(philo->table->start_time);
	if (philo->table->must_eat_count == 0)
	{
		sem_post(philo->sem_philo_full);
		exit(CHILD_EXIT_PHILO_FULL);
	}
	print_status(philo, "has taken a fork");
	philo_sleep(philo->table->time_to_die);
	print_status(philo, "died");
	free_table(philo->table);
	exit(CHILD_EXIT_PHILO_DEAD);
}

/* philosopher_routine:
*	Runs the philosopher process' routine as long as the philosopher
*	is alive. The philosopher will eat, sleep and think.
*/
static void	philosopher_routine(t_philo *philo)
{
	if (philo->id % 2)
		think_routine(philo, 1);
	while (1)
	{
		routine(philo);
		think_routine(philo, 0);
	}
}

/* philosopher:
*	The philosopher thread routine. The philosopher must eat, sleep
*	and think. In order to avoid conflicts between philosopher processes,
*	philosophers with an even id start by thinking, which delays their
*	meal time by a small margin. This allows odd-id philosophers to
*	grab both of their forks first, avoiding deadlocks.
*/
void	philosopher(t_table *table)
{
	t_philo	*philo;

	philo = table->this_philo;
	if (philo->table->n_philos == 1)
		lone_philo_routine(philo);
	init_philo_ipc(table, philo);
	if (philo->table->must_eat_count == 0)
	{
		sem_post(philo->sem_philo_full);
		child_exit(table, CHILD_EXIT_PHILO_FULL);
	}
	if (philo->table->time_to_die == 0)
	{
		sem_post(philo->sem_philo_dead);
		child_exit(table, CHILD_EXIT_PHILO_DEAD);
	}
	sem_wait(philo->sem_meal);
	philo->last_meal = philo->table->start_time;
	sem_post(philo->sem_meal);
	sim_start_delay(philo->table->start_time);
	philosopher_routine(philo);
}
