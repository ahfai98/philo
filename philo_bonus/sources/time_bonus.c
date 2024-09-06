/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 21:15:52 by jyap              #+#    #+#             */
/*   Updated: 2024/09/07 07:46:51 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

unsigned long	get_time_in_ms(struct timeval *time)
{
	struct timeval		tv;

	if (time != NULL)
		return (time->tv_sec * 1000 + time->tv_usec / 1000);
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	sim_start_delay(unsigned long start_time)
{
	while (get_time_in_ms(NULL) < start_time)
		usleep(100);
}

void	ft_usleep(int sleep_time)
{
	struct timeval	start;
	struct timeval	now;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&now, NULL);
		if (((now.tv_sec - start.tv_sec) * 1000)
			+ ((now.tv_usec - start.tv_usec) / 1000) >= sleep_time)
			break ;
		usleep(100);
	}
}

/* Gets the current time in int form */
int	get_time(struct timeval old)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (((time.tv_sec - old.tv_sec) * 1000)
		+ ((time.tv_usec - old.tv_usec) / 1000));
}
