/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 11:00:22 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/10 12:35:01 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <unistd.h> // for usleep()
#include <sys/time.h> // for gettimeofday


#include <stdio.h> // remove

/** Sleep for the amount of time provided by the user. */
void	philo_sleep(t_overview *overview)
{
	print_with_lock(overview, MSG_SLEEP);
	usleep(overview->time_to_sleep);
}

/** The philosopher locks both forks, waits, then updates the time since it
 * last ate, then unlocks the forks again.
 */
void	philo_eat(t_overview *overview, t_philosopher *philo)
{
	struct timeval	time_struct;

	// lock both forks	
	// phtread_mutex_lock(&mutex);
	// i think this is already done inside grab? 

	// print
	print_with_lock(overview, MSG_EAT); // pass philo?

	// eat
	usleep(overview->time_to_eat);

	gettimeofday(&time_struct, NULL);
	philo->recent_meal = time_struct.tv_usec;

	pthread_mutex_unlock(&overview->forks[philo->philo_nbr - 1]);
	pthread_mutex_unlock(&overview->forks[philo->philo_nbr + 1]);
}

// grab fork function
void philo_grab(t_overview *overview, t_philosopher *philo)
{
	if (philo->philo_nbr == philo->overview->amount_to_eat)
	{
		printf("special case!\n");
	}
	else
	{
		pthread_mutex_lock(&overview->forks[philo->philo_nbr - 1]);
		print_with_lock(overview, MSG_FORK);
		pthread_mutex_lock(&overview->forks[philo->philo_nbr + 1]);
		print_with_lock(overview, MSG_FORK);
	}
}

/** Function to run when a philosopher is sleeping. */
void	philo_think(t_overview *overview)
{
	print_with_lock(overview, MSG_THINK);
}

/** Handle philosopher death by waiting for the other philosophers to 
 * stop eating/sleeping, and then printing the death message.
 */
void	philo_die(t_overview *overview) // pass a name variable or whatever
{
	usleep(1000);

	// lock & open mutexes ? idk
	overview->someone_died = true;
	print_with_lock(overview, MSG_DIE);
}
