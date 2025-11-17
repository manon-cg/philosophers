/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 12:46:15 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/17 15:22:07 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/** Takes the arguments provided by the user and stores them in the overview
 * struct. Checks all are valid positive integers, except for amount_to_eat
 * which is also allowed to be 0.
 */
bool	handle_args(int argc, char *argv[], t_overview *overview)
{
	if (argc < 5 || argc > 6)
		return (print_error(MSG_ARG_STRUCTURE), false);
	overview->philo_count = philo_atoi(argv[1]);
	if (overview->philo_count <= 0)
		return (print_error(MSG_INVALID_INTS), false);
	overview->time_to_die = philo_atoi(argv[2]);
	if (overview->time_to_die <= 0)
		return (print_error(MSG_INVALID_INTS), false);
	overview->time_to_eat = philo_atoi(argv[3]);
	if (overview->time_to_eat <= 0)
		return (print_error(MSG_INVALID_INTS), false);
	overview->time_to_sleep = philo_atoi(argv[4]);
	if (overview->time_to_sleep <= 0)
		return (print_error(MSG_INVALID_INTS), false);
	overview->amount_to_eat = 0;
	overview->max_food_arg_given = false;
	if (argc > 5)
	{
		overview->amount_to_eat = philo_atoi(argv[5]);
		if (overview->amount_to_eat < 0)
			return (print_error(MSG_INVALID_INTS), false);
		overview->max_food_arg_given = true;
	}
	return (true);
}

/** Each philosopher struct is initialized with their needed values. */
bool	init_philo_info(t_overview *overview, t_philosopher *philos)
{
	int	i;

	i = 0;
	while (i < overview->philo_count)
	{
		(philos[i]).philo_nbr = i + 1;
		(philos[i]).alive = true;
		(philos[i]).hungry = true;
		(philos[i]).overview = overview;
		(philos[i]).meal_count = 0;
		(philos[i]).recent_meal = overview->start_time_msec; // cant move? :(
		if (pthread_mutex_init(&(philos[i]).recent_meal_lock, NULL) != 0)
			return (clean_philo_mutex_fail(philos, i,
					FAIL_INIT_RECENT_MEAL_MUTEX));
		if (pthread_mutex_init(&(philos[i]).hunger_lock, NULL) != 0)
			return (clean_philo_mutex_fail(philos, i, FAIL_INIT_HUNGER_MUTEX));
		i++;
	}
	return (true);
}

/** Create the threads to make philosophers. */
bool	create_philos(t_overview *overview, t_philosopher *philos)
{
	int	i;
	int	j;

	i = 0;
	overview->start_time_msec = get_time_msec(); // cant move this ig
	if (!init_philo_info(overview, philos))
		return (false);
	while (i < overview->philo_count)
	{
		if (pthread_create(&(philos[i]).philo_thread, NULL,
				philo_function, &philos[i]) != 0)
		{
			j = 0;
			while (j < i)
			{
				pthread_join((philos[i]).philo_thread, NULL);
				j++;
			}
			clean_philo_mutex_fail(philos, overview->philo_count, FAIL_THREAD);
			print_error("Failed to create philosopher thread.\n");
			return (false);
		}
		i++;
	}
	return (true);
}

/** Set up the shared mutexes and start time. */
bool	set_up(t_overview *overview)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&(overview->write_mutex), NULL) != 0)
		return (print_error("Write mutex initialisation failed.\n"), false);
	if (pthread_mutex_init(&(overview->done_mutex), NULL) != 0)
	{
		pthread_mutex_destroy(&(overview->write_mutex));
		return (print_error("Done mutex initialisation failed.\n"), false);
	}
	pthread_mutex_lock(&overview->done_mutex);
	overview->done = false;
	pthread_mutex_unlock(&overview->done_mutex);
	while (i < overview->philo_count)
	{
		if (pthread_mutex_init(&(overview->fork_mutexes[i]), NULL) != 0)
			return (clean_fork_init_fail(overview, i));
		overview->forks[i] = AVAILABLE;
		i++;
	}
	return (true);
}

/** Creates and locks the start_mutex, which will only be unlocked when all
 * philosopher threads are created and the program can start.
 */
bool	init_barrier(t_overview *overview)
{
	if (pthread_mutex_init(&(overview->start_mutex), NULL) != 0)
	{
		print_error("Start mutex initialisation failed.\n");
		return (false);
	}
	pthread_mutex_lock(&overview->start_mutex);
	return (true);
}
