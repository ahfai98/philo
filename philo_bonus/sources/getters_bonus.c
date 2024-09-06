/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 20:32:24 by jyap              #+#    #+#             */
/*   Updated: 2024/09/07 07:16:59 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* Prints out messages based on events */
void	get_message(t_philo *philo, int i, char *str, int end)
{
	int	time;

	sem_wait(philo->write);
	time = get_time(philo->start_time);
	printf("%d\t%d %s\n", time, i, str);
	if (end != 1)
		sem_post(philo->write);
}
