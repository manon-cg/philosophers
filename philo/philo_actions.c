/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 11:00:22 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/07 10:54:48 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <sys/time.h> // for gettimeofday

/** Sleep for the amount of time provided by the user. */
void	philo_sleep(t_overview *overview)
{
	// mutex
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

	// print
	print_with_lock(overview, MSG_EAT); // pass philo?

	// eat
	usleep(overview->time_to_eat);

	gettimeofday(&time_struct, NULL);
	philo->recent_meal = time_struct.tv_usec;

	// unlock forks
}

// grab fork function
void philo_grab(t_overview *overview)
{
	// lock

	// ???? grab ??? ? ? ?
	print_with_lock(overview, MSG_FORK);
}

// think function
void	philo_think(t_overview *overview)
{
	print_with_lock(overview, MSG_THINK);
}

/** Handle philosopher death by waiting for the other philosophers to 
 * stop eating/sleeping, and then printing the death message.
 */
void	philo_die(void) // pass a name variable or whatever
{
	usleep(1000);
	// lock & open mutexes ? idk
	printf("[timestamp] [philo nbr] %s\n", MSG_DIE);
}
