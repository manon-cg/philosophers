/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 11:00:22 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/17 16:57:47 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <unistd.h> // for usleep()
#include <sys/time.h> // for gettimeofday

/** Sleep for the amount of time provided by the user. */
void	philo_sleep(t_overview *overview, t_philosopher *philo)
{
	print_with_lock(overview, philo, MSG_SLEEP);
	usleep(overview->time_to_sleep * 1000);
}

/** The philosopher locks both forks, waits, then updates the time since it
 * last ate, then unlocks the forks again.
 */
bool	philo_eat(t_overview *overview, t_philosopher *philo)
{
	if (!philo_grab(overview, philo))
		return (false);
	if (!check_keep_going(overview) || philo_death_check(philo))
		return (drop_both_forks(overview, philo), false);
	print_with_lock(overview, philo, MSG_EAT);
	pthread_mutex_lock(&philo->recent_meal_lock);
	philo->recent_meal = get_time_msec();
	philo->meal_count += 1;
	pthread_mutex_unlock(&philo->recent_meal_lock);
	if (!check_keep_going(overview) || philo_death_check(philo))
		return (drop_both_forks(overview, philo), false);
	usleep(overview->time_to_eat * 1000);
	drop_both_forks(overview, philo);
	if (overview->max_food_arg_given)
		philo_full_check(philo);
	return (true);
}

/** Philosopher grabs two forks by locking the mutexes and changing the fork
 * values to TAKEN.
 */
bool	philo_grab(t_overview *overview, t_philosopher *philo)
{
	if (!check_keep_going(overview) || philo_death_check(philo))
		return (false);
	if (philo->philo_nbr % 2 == 0)
		take_fork(overview, philo->philo_nbr % overview->philo_count);
	else
		take_fork(overview, philo->philo_nbr - 1);
	print_with_lock(overview, philo, MSG_FORK);
	if (!check_keep_going(overview) || philo_death_check(philo))
	{
		if (philo->philo_nbr % 2 == 0)
			drop_single_fork(overview, philo->philo_nbr % \
overview->philo_count);
		else
			drop_single_fork(overview, philo->philo_nbr - 1);
		return (false);
	}
	if (philo->philo_nbr % 2 == 0)
		take_fork(overview, philo->philo_nbr - 1);
	else
		take_fork(overview, philo->philo_nbr % overview->philo_count);
	print_with_lock(overview, philo, MSG_FORK);
	return (true);
}

/** Function to run when a philosopher is thinking. */
void	philo_think(t_overview *overview, t_philosopher *philo)
{
	print_with_lock(overview, philo, MSG_THINK);
}

/** Handle philosopher death by printing the death message.
 */
void	*philo_die(t_overview *overview, t_philosopher *philo)
{
	pthread_mutex_lock(&(overview->done_mutex));
	overview->done = true;
	pthread_mutex_unlock(&(overview->done_mutex));
	print_with_lock(overview, philo, MSG_DIE);
	return (NULL);
}
