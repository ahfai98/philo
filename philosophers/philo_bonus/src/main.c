/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:31:24 by jyap              #+#    #+#             */
/*   Updated: 2024/08/23 19:51:57 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "philo_bonus.h"

/* has_simulation_stopped:
*	Checks if the simulation is about to end.
*	Returns 1 if the simulation must stop, 0 if not.
*/
int	has_sim_stopped(t_table *t)
{
	int	ret;

	sem_wait(t->sem_stop);
	ret = t->stop_sim;
	sem_post(t->sem_stop);
	return (ret);
}

/* start_simulation:
*	Launches the simulation by creating a new child process for each
*	philosopher. The process ids are recorded to be able to wait for each
*	child in turn, and send them a kill signal if need be.
*	Also creates a grim reaper thread to monitor philosophers and detect
*	if everyone has eaten enough.
*
*	Returns 1 if the simulation was successfully started, 0 if there
*	was an error.
*/
static int	start_sim(t_table *t)
{
	int	i;
	pid_t			pid;

	t->start_time = get_time_in_ms() + (t->n_philos * 10);
	i = -1;
	while (++i < t->n_philos)
	{
		pid = fork();
		if (pid == -1)
			return (error_failure(STR_ERR_FORK, NULL, t));
		else if (pid > 0)
			t->pids[i] = pid;
		else if (pid == 0)
		{
			t->this_philo = t->philos[i];
			philosopher(t);
		}
	}
	if (start_grim_reaper_threads(t) == 0)
		return (0);
	return (1);
}

/* get_child_philo:
*	Waits for a philosopher process to exit. If the philo process
*	exits with an error or a dead philosopher, sends the signal to
*	kill all other child processes.
*/
static int	get_child_philo(t_table *t, pid_t *pid)
{
	int	philo_exit_code;
	int	exit_code;

	if (*pid && waitpid(*pid, &philo_exit_code, WNOHANG) != 0)
	{
		if (WIFEXITED(philo_exit_code))
		{
			exit_code = WEXITSTATUS(philo_exit_code);
			if (exit_code == CHILD_EXIT_PHILO_DEAD)
				return (kill_all_philos(t, 1));
			if (exit_code == CHILD_EXIT_ERR_PTHREAD
				|| exit_code == CHILD_EXIT_ERR_SEM)
				return (kill_all_philos(t, -1));
			if (exit_code == CHILD_EXIT_PHILO_FULL)
			{
				t->philo_full_count += 1;
				return (1);
			}
		}
	}
	return (0);
}

/* stop_simulation:
*	Waits for each child process and analyses its exit code
*	as long as the simulation is still ongoing.
*	Ends the simulation when one of the end conditions are fulfilled:
*	when a philosopher dies or when all philosophers have eaten enough.
*/
static int	stop_sim(t_table	*t)
{
	int	i;
	int				exit_code;

	sim_start_delay(t->start_time);
	while (has_sim_stopped(t) == 0)
	{
		i = 0;
		while (i < t->n_philos)
		{
			exit_code = get_child_philo(t, &t->pids[i]);
			if (exit_code == 1 || exit_code == -1)
			{
				sem_wait(t->sem_stop);
				t->stop_sim = 1;
				sem_post(t->sem_philo_full);
				sem_post(t->sem_philo_dead);
				sem_post(t->sem_stop);
				return (exit_code);
			}
			i++;
		}
	}
	return (0);
}

/* main:
*	Main philosophers function.
*/
int	main(int ac, char **av)
{
	t_table	*t;

	t = NULL;
	if (ac  < 5 || ac > 6)
		return (msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!is_valid_input(ac, av))
		return (EXIT_FAILURE);
	t = init_table(ac, av, 1);
	if (!t)
		return (EXIT_FAILURE);
	if (!start_sim(t))
		return (EXIT_FAILURE);
	if (stop_sim(t) == -1)
		return (table_cleanup(t, EXIT_FAILURE));
	return (table_cleanup(t, EXIT_SUCCESS));
}
