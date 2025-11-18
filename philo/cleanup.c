/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 12:41:05 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/18 10:05:26 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdbool.h>

/** Join all philosopher threads. */
void	join_philos(t_philosopher *philos)
{
	int	i;

	i = 0;
	while (i < philos->overview->philo_count)
	{
		if (pthread_join((philos[i]).philo_thread, NULL) != 0)
			print_error("Joining philosophers failed\n");
		i++;
	}
}

/** Prints an error and destroys all previously created fork mutexes. */
bool	clean_fork_init_fail(t_overview *overview, int i)
{
	int	j;

	j = 0;
	print_error("Fork mutex initialisation failed.\n");
	while (j < i)
	{
		pthread_mutex_destroy(&(overview->fork_mutexes[j]));
		j++;
	}
	return (false);
}

/** Prints an error and destroys all individual mutexes that have already been
 * created. */
bool	clean_philo_mutex_fail(t_philosopher *philos, int i, int error)
{
	int	j;

	if (error == FAIL_INIT_RECENT_MEAL_MUTEX)
		print_error("Recent meal mutex initialization failed.\n");
	if (error == FAIL_INIT_HUNGER_MUTEX)
		print_error("Hunger mutex initialization failed.\n");
	j = 0;
	while (j < i)
	{
		pthread_mutex_destroy(&(philos[j]).recent_meal_lock);
		pthread_mutex_destroy(&(philos[j]).hunger_lock);
		j++;
	}
	if (error == FAIL_INIT_HUNGER_MUTEX)
		pthread_mutex_destroy(&(philos[j]).recent_meal_lock);
	return (false);
}

/** Clean all the mutexes. If philos have not been initialised, don't destroy
 * the recent_meal_lock and hunger_lock.
*/
void	clean_up_mutexes(t_overview *overview, t_philosopher *philos,
	bool philos_init)
{
	int	i;

	pthread_mutex_destroy(&(overview->write_mutex));
	pthread_mutex_destroy(&(overview->done_mutex));
	i = 0;
	while (i < overview->philo_count)
	{
		pthread_mutex_destroy(&(overview->fork_mutexes[i]));
		if (philos_init)
		{
			pthread_mutex_destroy(&(philos[i]).recent_meal_lock);
			pthread_mutex_destroy(&(philos[i]).hunger_lock);
		}
		i++;
	}
	if (!philos_init)
		pthread_mutex_unlock(&overview->start_mutex);
	pthread_mutex_destroy(&overview->start_mutex);
}
