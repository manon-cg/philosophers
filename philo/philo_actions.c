/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 11:00:22 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/13 13:14:54 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <unistd.h> // for usleep()
#include <sys/time.h> // for gettimeofday

#include <stdio.h> // remove

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
	if (!philo_grab(philo->overview, philo))
		return (false);
	// if (philo_death_check(philo))
	// {
	// 	philo_die(overview, philo);
	// 	return (false);
	// }
	// if (overview->done)
	// 	return (false);
	print_with_lock(overview, philo, MSG_EAT);
	pthread_mutex_lock(&philo->recent_meal_lock);
	philo->recent_meal = get_time_msec();// error handing
	philo->meal_count += 1;
	pthread_mutex_unlock(&philo->recent_meal_lock);
	if (overview->done)
	{
		pthread_mutex_unlock(&overview->forks[philo->philo_nbr - 1]);
		pthread_mutex_unlock(&overview->forks[philo->philo_nbr % overview->philo_count]);
		return (false);
	}
	usleep(overview->time_to_eat * 1000);
	pthread_mutex_unlock(&overview->forks[philo->philo_nbr - 1]);
	pthread_mutex_unlock(&overview->forks[philo->philo_nbr % overview->philo_count]);
	if (overview->max_food_arg_given)
		philo_full_check(philo);
	return (true);
}

// grab fork function
bool philo_grab(t_overview *overview, t_philosopher *philo)
{
	if (philo->philo_nbr % 2 == 0)
	{
		// if (!check_keep_going(overview))
		// 	return (false);
		pthread_mutex_lock(&overview->forks[philo->philo_nbr % overview->philo_count]);
		// if (!check_keep_going(overview))
		// 	return (false);
		print_with_lock(overview, philo, MSG_FORK);
		// if (philo_death_check(philo))
		// {
		// 	pthread_mutex_unlock(&overview->forks[philo->philo_nbr % overview->philo_count]);
		// 	return (false);
		// }
		// if (!check_keep_going(overview))
		// {
		// 	pthread_mutex_unlock(&overview->forks[philo->philo_nbr % overview->philo_count]);
		// 	return (false);
		// }
		pthread_mutex_lock(&overview->forks[philo->philo_nbr - 1]);
		// if (philo_death_check(philo))
		// {
		// 	pthread_mutex_unlock(&overview->forks[philo->philo_nbr % overview->philo_count]);
		// 	return (false);
		// }
		// if (!check_keep_going(overview))
		// {
		// 	pthread_mutex_unlock(&overview->forks[philo->philo_nbr % overview->philo_count]);
		// 	return (false);
		// }
		print_with_lock(overview, philo, MSG_FORK);
	}
	else
	{
		// if (!check_keep_going(overview))
		// 	return (false);
		// if (philo_death_check(philo))
		// 	return (false);
		pthread_mutex_lock(&overview->forks[philo->philo_nbr - 1]);
		print_with_lock(overview, philo, MSG_FORK);
		// if (philo_death_check(philo))
		// {
		// 	pthread_mutex_lock(&overview->forks[philo->philo_nbr - 1]);
		// 	return (false);
		// }
		// if (!check_keep_going(overview))
		// {
		// 	pthread_mutex_lock(&overview->forks[philo->philo_nbr - 1]);
		// 	return (false);
		// }
		pthread_mutex_lock(&overview->forks[philo->philo_nbr % overview->philo_count]);
		print_with_lock(overview, philo, MSG_FORK);
	}
	return (true);
}

/** Function to run when a philosopher is thinking. */
void	philo_think(t_overview *overview, t_philosopher *philo)
{
	print_with_lock(overview, philo, MSG_THINK);
}

/** Handle philosopher death by waiting for the other philosophers to 
 * stop eating/sleeping, and then printing the death message.
 */
void	*philo_die(t_overview *overview, t_philosopher *philo)
{
	// pthread_mutex_lock(&(overview->done_mutex));
	// overview->done = true;
	// pthread_mutex_unlock(&(overview->done_mutex));
	print_with_lock(overview, philo, MSG_DIE);
	return (NULL);
}
