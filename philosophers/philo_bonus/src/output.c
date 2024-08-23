/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:31:28 by jyap              #+#    #+#             */
/*   Updated: 2024/08/23 19:52:49 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "philo_bonus.h"

/* print_status:
*	Prints a philosopher's status in plain text as required by the project
*	subject:
*		timestamp_in_ms X status
*/
void	print_status(t_philo *philo, char *str)
{
	printf("%d %d %s\n", get_time_in_ms() - philo->table->start_time,
		philo->id + 1, str);
}

/* write_status:
*	Prints the status of a philosopher. Decrements the write semaphore to
*	avoid intertwined messages from different processes. If the message comes
*	from a grim reaper, the write semaphore is not increased again after
*	the message is displayed in the terminal, to avoid any status messages
*	being displayed after a philosopher has died.
*
*	If DEBUG_FORMATTING is set to 1 in philo.h, the status will
*	be formatted with colors and extra information to help with debugging.
*	Otherwise the output will be the regular format required by the project
*	subject.
*/
void	write_status(t_philo *philo, int reaper_report, t_status status)
{
	sem_wait(philo->sem_write);
	if (status == DIED)
		print_status(philo, "died");
	else if (status == EATING)
		print_status(philo, "is eating");
	else if (status == SLEEPING)
		print_status(philo, "is sleeping");
	else if (status == THINKING)
		print_status(philo, "is thinking");
	else if (status == TAKEN_FORK || status == TAKEN_FORK)
		print_status(philo, "has taken a fork");
	if (!reaper_report)
		sem_post(philo->sem_write);
}

/* write_outcome:
*	Prints the outcome of the simulation if a number of times to
*	eat was specified. Only used for debug purposes.
*/
