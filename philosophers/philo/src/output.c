/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 13:20:23 by mcombeau          #+#    #+#             */
/*   Updated: 2024/08/23 10:29:48 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* print_status:
*	Prints a philosopher's status in plain text as required by the project
*	subject:
*		timestamp_in_ms X status
*/
static void	print_status(t_philo *philo, char *str)
{	
	printf("%d %d %s\n", get_time_difference_in_ms(philo->table->start_time),
		philo->id + 1, str);
}

/* write_status:
*	Prints the status of a philosopher as long as the simulation is
*	still active. Locks the write mutex to avoid intertwined messages
*	from different threads.
*
*	If DEBUG_FORMATTING is set to 1 in philo.h, the status will
*	be formatted with colors and extra information to help with debugging.
*	Otherwise the output will be the regular format required by the project
*	subject.
*/
void	write_status(t_philo *philo, int reaper_report, t_status status)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if (has_sim_stopped(philo->table) == 1 && reaper_report == 0)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (status == DIED)
		print_status(philo, "died");
	else if (status == EATING)
		print_status(philo, "is eating");
	else if (status == SLEEPING)
		print_status(philo, "is sleeping");
	else if (status == THINKING)
		print_status(philo, "is thinking");
	else if (status == TAKEN_FORK)
		print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->table->write_lock);
}
