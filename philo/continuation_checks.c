/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continuation_checks.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:07:12 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/17 16:08:50 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/** Checks whether the 'done' bool has been set to true. Has to lock and 
 * unlock the done_mutex.
 * @param overview Struct with info about the program.
 */
bool	check_keep_going(t_overview *overview)
{
	pthread_mutex_lock(&(overview->done_mutex));
	if (overview->done)
	{
		pthread_mutex_unlock(&(overview->done_mutex));
		return (false);
	}
	pthread_mutex_unlock(&(overview->done_mutex));
	return (true);
}

/** Checks whether a philosopher has died by comparing time since most
 * recent meal to time to die (both in msec).
 */
bool	philo_death_check(t_philosopher *philo)
{
	unsigned int	current_time;
	t_overview		*overview;

	current_time = get_time_msec(); // err?
	overview = philo->overview;
	pthread_mutex_lock(&(philo->recent_meal_lock));
	if (current_time - philo->recent_meal >= \
(unsigned int)philo->overview->time_to_die)
	{
		pthread_mutex_unlock(&(philo->recent_meal_lock));
		pthread_mutex_lock(&(overview->done_mutex));
		overview->done = true;
		pthread_mutex_unlock(&(overview->done_mutex));
		return (true);
	}
	pthread_mutex_unlock(&(philo->recent_meal_lock));
	return (false);
}

/** Check if all philosophers have eaten the proper amount of meals. */
bool	philo_full_check(t_philosopher *philo)
{
	if (philo->meal_count == philo->overview->amount_to_eat)
	{
		pthread_mutex_lock(&philo->hunger_lock);
		philo->hungry = false;
		pthread_mutex_unlock(&philo->hunger_lock);
		return (false);
	}
	return (true);
}

/** Uses the 'hungry' variable of each individual philosopher to see if all
 * of them are no longer hungry. If so, sets the 'done' variable to true.
 */
bool	all_full_check(t_philosopher *philos, t_overview *overview)
{
	int	i;

	i = 0;
	while (i < overview->philo_count)
	{
		pthread_mutex_lock(&philos[i].hunger_lock);
		if (philos[i].hungry == true)
			return (pthread_mutex_unlock(&philos[i].hunger_lock), false);
		pthread_mutex_unlock(&philos[i].hunger_lock);
		i++;
	}
	pthread_mutex_lock(&(overview->done_mutex));
	overview->done = true;
	pthread_mutex_unlock(&(overview->done_mutex));
	return (true);
}
