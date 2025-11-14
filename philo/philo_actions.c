/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 11:00:22 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/14 16:19:55 by mgroos           ###   ########.fr       */
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

/** Drops both forks the philosopher is holding, setting them to
 * available and unlocking them. */
void	drop_forks(t_overview *overview, t_philosopher *philo)
{
	overview->forks[philo->philo_nbr - 1] = AVAILABLE;
	pthread_mutex_unlock(&overview->fork_mutexes[philo->philo_nbr - 1]);
	overview->forks[philo->philo_nbr % overview->philo_count] = AVAILABLE;
	pthread_mutex_unlock(&overview->fork_mutexes[philo->philo_nbr % \
overview->philo_count]);
}

/** The philosopher locks both forks, waits, then updates the time since it
 * last ate, then unlocks the forks again.
 */
bool	philo_eat(t_overview *overview, t_philosopher *philo)
{
	if (!philo_grab(overview, philo))
		return (false);
	if (!check_keep_going(overview) || philo_death_check(philo))
		return (drop_forks(overview, philo), false);
	print_with_lock(overview, philo, MSG_EAT);
	pthread_mutex_lock(&philo->recent_meal_lock);
	philo->recent_meal = get_time_msec(); // error handing
	philo->meal_count += 1;
	pthread_mutex_unlock(&philo->recent_meal_lock);
	if (!check_keep_going(overview) || philo_death_check(philo))
		return (drop_forks(overview, philo), false);
	usleep(overview->time_to_eat * 1000);
	drop_forks(overview, philo);
	if (overview->max_food_arg_given)
		philo_full_check(philo);
	return (true);
}

// make functions for lock right fork & lock left fork? or some bool situation?

/** Philosopher grabs two forks by locking the mutexes and changing the fork
 * values to TAKEN.
 */
bool philo_grab(t_overview *overview, t_philosopher *philo)
{
	if (!check_keep_going(overview) || philo_death_check(philo))
		return (false);
	if (philo->philo_nbr % 2 == 0)
	{
		pthread_mutex_lock(&overview->fork_mutexes[philo->philo_nbr % \
	overview->philo_count]);
		overview->forks[philo->philo_nbr % overview->philo_count] = TAKEN;
	}
	else
	{
		pthread_mutex_lock(&overview->fork_mutexes[philo->philo_nbr - 1]);
		overview->forks[philo->philo_nbr - 1] = TAKEN;
	}
	print_with_lock(overview, philo, MSG_FORK);
	if (!check_keep_going(overview) || philo_death_check(philo))
	{
		if (philo->philo_nbr % 2 == 0)
		{
			overview->forks[philo->philo_nbr % overview->philo_count] = AVAILABLE;
			pthread_mutex_unlock(&overview->fork_mutexes[philo->philo_nbr % \
	overview->philo_count]);
		}
		else
		{
			overview->forks[philo->philo_nbr - 1] = AVAILABLE;
			pthread_mutex_unlock(&overview->fork_mutexes[philo->philo_nbr - 1]);
		}
		return (false);
	}
	if (philo->philo_nbr % 2 == 0)
	{
		pthread_mutex_lock(&overview->fork_mutexes[philo->philo_nbr - 1]);
	overview->forks[philo->philo_nbr - 1] = TAKEN;
	print_with_lock(overview, philo, MSG_FORK);
	}
	else
	{
		pthread_mutex_lock(&overview->fork_mutexes[philo->philo_nbr % \
	overview->philo_count]);
		overview->forks[philo->philo_nbr % overview->philo_count] = TAKEN;
	}
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
