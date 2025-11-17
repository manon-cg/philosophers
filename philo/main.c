/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:39:04 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/17 16:56:49 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h> // for printf
#include <stdlib.h> // for malloc/free
#include <stddef.h> // for size_t
#include <pthread.h>  // for threading
#include <unistd.h> // for usleep()

/** Functions for when a philosopher is all alone. */
void	*lonely_philo_function(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->overview->fork_mutexes[0]);
	philo->overview->forks[0] = TAKEN;
	print_with_lock(philo->overview, philo, MSG_FORK);
	usleep(philo->overview->time_to_die);
	philo->overview->forks[0] = AVAILABLE;
	pthread_mutex_unlock(&philo->overview->fork_mutexes[0]);
	return (NULL);
}

/** Philo function ran by each philosopher. First it has to wait for the 
 * start mutex to be unlocked, then it can run its eat / sleep / think loop.
*/
void	*philo_function(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	pthread_mutex_lock(&philo->overview->start_mutex);
	pthread_mutex_unlock(&philo->overview->start_mutex);
	if (philo->overview->philo_count == 1)
		return (lonely_philo_function(philo));
	while (check_keep_going(philo->overview))
	{
		if (philo->philo_nbr % 2 == 1)
			usleep(1000);
		if (!check_keep_going(philo->overview) || philo_death_check(philo))
			return (NULL);
		if (!philo_eat(philo->overview, philo))
			return (NULL);
		if (!philo->hungry)
			return (NULL);
		if (!check_keep_going(philo->overview) || philo_death_check(philo))
			return (NULL);
		philo_sleep(philo->overview, philo);
		if (!check_keep_going(philo->overview) || philo_death_check(philo))
			return (NULL);
		philo_think(philo->overview, philo);
	}
	return (NULL);
}

/** Function ran by the main non-philosopher thread.
 * @param overview Struct with info about the program.
 * @param philos Struct array containing all individual philosophers.
*/
void	*watcher_function(t_overview *overview, t_philosopher *philos)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < overview->philo_count)
		{
			if (philo_death_check(&philos[i]) == true)
				return (philo_die(overview, &philos[i]));
			if (all_full_check(philos, overview))
				return (NULL);
			i++;
		}
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_overview		overview;
	t_philosopher	philos[MAX_PHILO_COUNT];

	if (!handle_args(argc, argv, &overview))
		return (EXIT_FAILURE);
	if (!init_barrier(&overview))
		return (EXIT_FAILURE);
	if (!set_up(&overview))
		return (pthread_mutex_unlock(&overview.start_mutex),
			pthread_mutex_destroy(&overview.start_mutex), EXIT_FAILURE);
	if (!create_philos(&overview, philos))
		return (clean_up(&overview, philos, false), EXIT_FAILURE);
	pthread_mutex_unlock(&overview.start_mutex);
	watcher_function(&overview, philos);
	join_philos(philos);
	clean_up(&overview, philos, true);
	return (EXIT_SUCCESS);
}
