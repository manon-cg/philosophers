/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:39:04 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/10 12:48:08 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h> // for printf
#include <stdlib.h> // for malloc/free
#include <stddef.h> // for size_t
#include <pthread.h>  // for threading
# include <unistd.h> // for usleep()

/** Checks whether a philosopher has died by comparing time since most
 * recent meal to time to die (both in msec).
 */
bool	philo_death_check(t_philosopher *philo)
{
	unsigned int	current_time;

	printf("running a death check!\n");
	current_time = get_time_msec();
	// erorr handling
	if (current_time - philo->recent_meal >= (unsigned int)philo->overview->time_to_die)
	{
		return (true);
	}
	return (false);
}

/** Philo function */
void	*philo_function(void *arg)
{
	t_philosopher *philo;

	philo = (t_philosopher *)arg;
	printf("inside philo func\n");
	while (!philo_death_check(philo))
	{
		printf("running.....\n");
		philo_grab(philo->overview, philo);
		philo_eat(philo->overview, philo);
		philo_sleep(philo->overview);
		philo_think(philo->overview);
		usleep(1000);
	}
	return (NULL);
}

void	*watcher_function(void *arg)
{
	t_overview	*overview;

	overview = (t_overview *)arg;
	while (1)
	{
		if (overview->someone_died == true)
			break ;
	}
	return (NULL);
}

void	arg_error(void)
{
	// use fdputstr to put this on stderr instead?

	printf("Make sure to structure your input as follows:\n");
	printf("./philo [number_of_philosophers] [time_to_die] [time_to_eat] \
[time_to_sleep] [OPTIONAL: number_of_times_each_philosopher_must_eat]\n");
}

/** Takes the arguments provided by the user and stores them in the overview
 * struct. Checks all are valid positive integers, except for amount_to_eat
 * which is also allowed to be 0.
 */
bool	handle_args(int argc, char *argv[], t_overview *overview)
{
	if (argc < 5 || argc > 6)
		return (arg_error(), false);
	overview->philo_count = philo_atoi(argv[1]);
	if (overview->philo_count <= 0)
		return (printf(MSG_INVALID_INPUT), false);
	overview->time_to_die = philo_atoi(argv[2]);
	if (overview->time_to_die <= 0)
		return (printf(MSG_INVALID_INPUT), false);
	overview->time_to_eat = philo_atoi(argv[3]);
	if (overview->time_to_eat <= 0)
		return (printf(MSG_INVALID_INPUT), false);
	overview->time_to_sleep = philo_atoi(argv[4]);
	if (overview->time_to_sleep <= 0)
		return (printf(MSG_INVALID_INPUT), false);
	overview->amount_to_eat = 0;
	overview->end_point_given = false;	
	if (argc > 5)
	{
		overview->amount_to_eat = philo_atoi(argv[5]);
		if (overview->amount_to_eat < 0)
			return (printf(MSG_INVALID_INPUT), false);
		overview->end_point_given = true;
	}
	return (true);
}

/** Create the threads to make philosophers. */
void	create_philos(t_overview *overview, t_philosopher *philos)
{
	int	i;

	i = 0;
	while (i < overview->philo_count)
	{
		(philos[i]).philo_nbr = i + 1;
		(philos[i]).alive = true;
		(philos[i]).overview = overview;
		(philos[i]).recent_meal = 0;
		printf("creating a thread for philo: %i\n", (philos[i]).philo_nbr);
		pthread_create(&(philos[i]).philo_thread, NULL, philo_function, &philos[i]);
		// err handling
		i++;
	}
}

/** Set up the mutexes. */
void	set_up(t_overview *overview)
{
	int	i;

	i = 0;
	pthread_mutex_init(&(overview->write_mutex), NULL);
	// err

	// set up watcher thread
	pthread_create(&(overview->watcher), NULL, watcher_function, &overview);
	// err
	// pthread_join((overview->watcher), NULL);
	
	// set up forks
	while (i < overview->philo_count)
	{
		pthread_mutex_init(&(overview->forks[i]), NULL);
		i++;
	}
}

/** Clean up the mutexes. */
void	clean_up(t_overview *overview)
{
	pthread_mutex_destroy(&(overview->write_mutex));
	// other mutexes destroy

}

int	main(int argc, char *argv[])
{
	t_overview		overview;
	t_philosopher	philos[MAX_PHILO_COUNT];

	// printf("going to handle args:\n");
	if (!handle_args(argc, argv, &overview))
		return (EXIT_FAILURE);
	overview.start_time_msec = get_time_msec();
	print_overview(&overview);
	// fail handle ???
	set_up(&overview);
	create_philos(&overview, philos);
	clean_up(&overview);
	return (EXIT_SUCCESS);
}
