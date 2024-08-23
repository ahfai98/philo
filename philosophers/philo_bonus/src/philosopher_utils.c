/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:31:40 by jyap              #+#    #+#             */
/*   Updated: 2024/08/23 19:31:41 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "philo_bonus.h"

/* grab_fork:
*	Wait for the fork semaphore to allow the philosopher to take a
*	fork. When the philosopher takes a fork, the semaphore is decreased
*	by 1 to represent the fact that there is one less fork available
*	in the pool of forks.
*	There is no real distinction between the first and second fork a
*	philosopher takes, the dictinction is made here only for debut purposes.
*/
void	grab_fork(t_philo *philo)
{
	sem_wait(philo->sem_forks);
	sem_wait(philo->sem_meal);
	if (philo->nb_forks_held <= 0)
		write_status(philo, 0, TAKEN_FORK);
	if (philo->nb_forks_held == 1)
		write_status(philo, 0, TAKEN_FORK);
	philo->nb_forks_held += 1;
	sem_post(philo->sem_meal);
}
