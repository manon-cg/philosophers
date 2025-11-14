/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:39:04 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/14 15:31:54 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h> // for printf
#include <stdlib.h> // for malloc/free
#include <stddef.h> // for size_t
#include <pthread.h>  // for threading
# include <unistd.h> // for usleep()

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

	// printf("running a death check!\n");
	current_time = get_time_msec();
	overview = philo->overview;
	// erorr handling
// 	printf("%i: current: %d // recent meal: %d diff: %d\n", 
// philo->philo_nbr, current_time, philo->recent_meal, current_time - philo->recent_meal);
	pthread_mutex_lock(&(philo->recent_meal_lock));
	if (current_time - philo->recent_meal >= (unsigned int)philo->overview->time_to_die)
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
		philo->hungry = false;
		return (false);
	}
	return (true);
}

/** Functions for when a philosopher is all alone. */
void	*lonely_philo_function(t_philosopher *philo)
{
	// actually grab ? for pretend?
	print_with_lock(philo->overview, philo, MSG_FORK);
	usleep(philo->overview->time_to_die);
	return (NULL);
}

/** Philo function */
void	*philo_function(void *arg)
{
	t_philosopher *philo;

	philo = (t_philosopher *)arg;
	pthread_mutex_lock(&philo->overview->start_mutex);
	pthread_mutex_unlock(&philo->overview->start_mutex);
	if (philo->overview->philo_count == 1)
		return(lonely_philo_function(philo));
	while (check_keep_going(philo->overview))
	{
		if (philo->philo_nbr % 2 == 1)
		{
			usleep(200);
			// usleep((philo->overview->time_to_die - get_time_msec() - philo->recent_meal) - philo->overview->time_to_eat / 2);
		}
		else
			usleep(20);
		if (!philo_eat(philo->overview, philo))
			return (NULL);
		if (!philo->hungry)
			return (NULL);
		// if (!check_keep_going(philo->overview))
		// 	return (print_with_lock(philo->overview, philo, "EXITING\n"), NULL);
		philo_sleep(philo->overview, philo);
		// if (!check_keep_going(philo->overview))
		// 	return (print_with_lock(philo->overview, philo, "EXITING\n"), NULL);
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
			if (!(philos[i]).hungry)
			{
				pthread_mutex_lock(&(overview->done_mutex));
				overview->done = true;
				pthread_mutex_unlock(&(overview->done_mutex));
				return (NULL);
			}
			if (philo_death_check(&philos[i]) == true)
			{
				print_with_lock(overview, &(philos[i]), "FOUND DEAD IN WATCHER\n");
				return (philo_die(overview, &philos[i]));
			}
			i++;
		}
	}
	return (NULL);
}

/** Print a message on how to properly use philosophers. 
 * @param message String about either correct philosopher argument order, or 
 * about how the arguments should be positive ints.
*/
void	arg_error(char *message)
{
	write(2, message, ft_strlen(message));
}

/** Takes the arguments provided by the user and stores them in the overview
 * struct. Checks all are valid positive integers, except for amount_to_eat
 * which is also allowed to be 0.
 */
bool	handle_args(int argc, char *argv[], t_overview *overview)
{
	if (argc < 5 || argc > 6)
		return (arg_error(MSG_ARG_STRUCTURE), false);
	overview->philo_count = philo_atoi(argv[1]);
	if (overview->philo_count <= 0)
		return (arg_error(MSG_INVALID_INTS), false);
	overview->time_to_die = philo_atoi(argv[2]);
	if (overview->time_to_die <= 0)
		return (arg_error(MSG_INVALID_INTS), false);
	overview->time_to_eat = philo_atoi(argv[3]);
	if (overview->time_to_eat <= 0)
		return (arg_error(MSG_INVALID_INTS), false);
	overview->time_to_sleep = philo_atoi(argv[4]);
	if (overview->time_to_sleep <= 0)
		return (arg_error(MSG_INVALID_INTS), false);
	overview->amount_to_eat = 0;
	overview->max_food_arg_given = false;	
	if (argc > 5)
	{
		overview->amount_to_eat = philo_atoi(argv[5]);
		if (overview->amount_to_eat < 0)
			return (arg_error(MSG_INVALID_INTS), false);
		overview->max_food_arg_given = true;
	}
	return (true);
}

/** Each philosopher struct is initialized with their needed values. */
void	init_philo_info(t_overview *overview, t_philosopher *philos)
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
		(philos[i]).recent_meal = overview->start_time_msec;
		pthread_mutex_init(&(philos[i]).recent_meal_lock, NULL);
		// err handling
		i++;
	}
}

/** Create the threads to make philosophers. */
void	create_philos(t_overview *overview, t_philosopher *philos)
{
	int	i;

	i = 0;
	overview->start_time_msec = get_time_msec();
	init_philo_info(overview, philos);
	while (i < overview->philo_count)
	{
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
	pthread_mutex_init(&(overview->write_mutex), NULL); 	// err
	pthread_mutex_init(&(overview->done_mutex), NULL); 	// err
	pthread_mutex_lock(&overview->done_mutex);
	overview->done = false;
	pthread_mutex_unlock(&overview->done_mutex);
	while (i < overview->philo_count)
	{
		pthread_mutex_init(&(overview->fork_mutexes[i]), NULL);
		overview->forks[i] = AVAILABLE;
		i++;
	}
}

/** Clean up the mutexes. */
void	clean_up(t_overview *overview, t_philosopher *philos)
{
	int	i;

	pthread_mutex_destroy(&(overview->write_mutex));
	pthread_mutex_destroy(&(overview->done_mutex));
	i = 0;
	while (i < overview->philo_count)
	{
		pthread_mutex_destroy(&(overview->fork_mutexes[i]));
		pthread_mutex_destroy(&(philos[i]).recent_meal_lock);
		i++;
	}
	pthread_mutex_destroy(&overview->start_mutex);
}

void	join_philos(t_philosopher *philos)
{
	int	i;

	i = 0;
	while (i < philos->overview->philo_count)
	{
		// printf("tryig to join: %i\n", i + 1);
		usleep(1000);
		pthread_join((philos[i]).philo_thread, NULL);
		// err handling
		i++;
	}
}

/** Creates and locks the start_mutex, which will only be unlocked when all
 * philosopher threads are created and the program can start.
 */
bool	init_barrier(t_overview *overview)
{
	if (pthread_mutex_init(&(overview->start_mutex), NULL) != 0)
	{
		// msg about mutex init failure?
		return (false);
	}
	pthread_mutex_lock(&overview->start_mutex);
	return (true);
}

int	main(int argc, char *argv[])
{
	t_overview		overview;
	t_philosopher	philos[MAX_PHILO_COUNT];

	if (!handle_args(argc, argv, &overview))
		return (EXIT_FAILURE);
	init_barrier(&overview);
	create_philos(&overview, philos);
	set_up(&overview);
	pthread_mutex_unlock(&overview.start_mutex);
	watcher_function(&overview, philos);
	join_philos(philos);
	clean_up(&overview, philos);
	return (EXIT_SUCCESS);
}
