/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:55:26 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/17 15:57:23 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/** Sets the fork's value to AVAILABLE and unlocks its mutex. */
void	drop_single_fork(t_overview *overview, int fork_nbr)
{
	overview->forks[fork_nbr] = AVAILABLE;
	pthread_mutex_unlock(&overview->fork_mutexes[fork_nbr]);
}

/** Locks the fork's mutex and sets its value to TAKEN. */
void	take_fork(t_overview *overview, int fork_nbr)
{
	pthread_mutex_lock(&overview->fork_mutexes[fork_nbr]);
	overview->forks[fork_nbr] = TAKEN;
}

/** Drops both forks the philosopher is holding, setting them to
 * available and unlocking them. */
void	drop_both_forks(t_overview *overview, t_philosopher *philo)
{
	overview->forks[philo->philo_nbr - 1] = AVAILABLE;
	pthread_mutex_unlock(&overview->fork_mutexes[philo->philo_nbr - 1]);
	overview->forks[philo->philo_nbr % overview->philo_count] = AVAILABLE;
	pthread_mutex_unlock(&overview->fork_mutexes[philo->philo_nbr % \
overview->philo_count]);
}
