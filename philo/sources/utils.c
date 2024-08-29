/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 17:20:46 by jyap              #+#    #+#             */
/*   Updated: 2024/08/29 17:20:46 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	assign_forks(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->table->n_philos;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->table->n_philos;
		philo->fork[1] = philo->id;
	}
}
