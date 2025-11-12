/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:39:04 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/12 16:16:56 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h> // for printf
#include <stdlib.h> // for malloc/free
#include <stddef.h> // for size_t
#include <pthread.h>  // for threading
# include <unistd.h> // for usleep()

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
	if (current_time - philo->recent_meal >= (unsigned int)philo->overview->time_to_die)
	{
		pthread_mutex_lock(&(overview->done_mutex));
		overview->done = true;
		pthread_mutex_unlock(&(overview->done_mutex));
		return (true);
	}
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
void	*solo_philo(t_philosopher *philo)
{
	printf("grab a fork\n");
	usleep(philo->overview->time_to_die);
	philo->overview->done = true;
	printf("died! whoops\n");
	return (NULL);
}

/** Philo function */
void	*philo_function(void *arg)
{
	t_philosopher *philo;

	philo = (t_philosopher *)arg;
	if (philo->overview->philo_count == 1)
		return(solo_philo(philo));
	while (!philo->overview->done)
	{
		// if (philo_death_check(philo))
		// 	return (philo_die(philo->overview, philo));
		if (philo->philo_nbr % 2 == 1)
			usleep(1000);
		if (!check_keep_going(philo->overview))
			return (print_with_lock(philo->overview, philo, "EXITING\n"), NULL);
		if (!philo_eat(philo->overview, philo))
			break ;
		if (!check_keep_going(philo->overview))
			return (print_with_lock(philo->overview, philo, "EXITING\n"), NULL);
		if (!philo->hungry)
			break ;
		if (!check_keep_going(philo->overview))
			return (print_with_lock(philo->overview, philo, "EXITING\n"), NULL);
		philo_sleep(philo->overview, philo);
		if (!check_keep_going(philo->overview))
			return (print_with_lock(philo->overview, philo, "EXITING\n"), NULL);
		philo_think(philo->overview, philo);
	}
	return (NULL);
}

// /** Function ran by the watcher thread. */
// void	*watcher_function(void *arg)
// {
// 	t_overview		*overview;
// 	t_philosopher	*philos;

// 	// overview = (t_overview *)arg;
// 	philos = (t_philosopher *)arg;
// 	overview = (philos[0]).overview;
// 	// int	i;
// 	while (1)
// 	{
// 		// i = 0;
// 		// while (i <= overview->philo_count)
// 		// {
// 		// 	if (philo_death_check(&philos[i]) == true)
// 		// 		philo_die(overview, &philos[i]);
// 		// 	i++;
// 		// }

// 		pthread_mutex_lock(&overview->done_mutex);
// 		if (overview->done == true)
// 		{
// 			// pthread_mutex_lock(&overview->write_mutex);
// 			// printf("%d DEATH FOUND BY WATCHER!\n", get_time_msec() - overview->start_time_msec);
// 			// pthread_mutex_unlock(&overview->write_mutex);
// 			pthread_mutex_unlock(&overview->done_mutex);
// 			return (NULL) ;
// 		}
// 		pthread_mutex_unlock(&overview->done_mutex);
// 		// if (overview->done == true)
// 		// 	break;
// 		usleep(1000);
// 	}
// 	return (NULL);
// }

// /** Function ran by the watcher thread. */
// void	*watcher_function(void *arg)
// {
// 	t_overview		*overview;
// 	t_philosopher	*philos;
// 	// int				philo_count;

// 	philos = (t_philosopher *)arg;
// 	usleep(1000);
// 	overview = (philos[0]).overview;
// 	// philo_count = philos->overview->philo_count;
// 	int	i;
// 	while (1)
// 	{
// 		i = 0;
// 		while (i < overview->philo_count)
// 		{
// 			if (!(philos[i]).hungry)
// 			{
// 				print_with_lock(overview, &(philos[i]), "FOUND ATE ENOUGH IN WATCHER\n");
// 				pthread_mutex_lock(&(overview->done_mutex));
// 				overview->done = true;
// 				pthread_mutex_unlock(&(overview->done_mutex));
// 				return (NULL);
// 			}
// 			if (philo_death_check(&philos[i]) == true)
// 			{
// 				print_with_lock(overview, &(philos[i]), "FOUND DEAD IN WATCHER\n");
// 				return (philo_die(overview, &philos[i]));
// 			}
// 			i++;
// 		}

// 		// printf("watcher check: %i\n", philo_count);

// 		// pthread_mutex_lock(&overview->done_mutex);
// 		// if (overview->done == true)
// 		// {
// 		// 	// pthread_mutex_lock(&overview->write_mutex);
// 		// 	// printf("%d DEATH FOUND BY WATCHER!\n", get_time_msec() - overview->start_time_msec);
// 		// 	// pthread_mutex_unlock(&overview->write_mutex);
// 		// 	pthread_mutex_unlock(&overview->done_mutex);
// 		// 	return (NULL) ;
// 		// }
// 		// pthread_mutex_unlock(&overview->done_mutex);
// 		// if (overview->done == true)
// 		// 	break;
// 	}
// 	return (NULL);
// }

/** Function ran by the main thread. */
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
				print_with_lock(overview, &(philos[i]), "FOUND ATE ENOUGH IN WATCHER\n");
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

/** Print a message on how to properly use philosophers. */
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
	overview->max_food_arg_given = false;	
	if (argc > 5)
	{
		overview->amount_to_eat = philo_atoi(argv[5]);
		if (overview->amount_to_eat < 0)
			return (printf(MSG_INVALID_INPUT), false);
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
		(philos[i]).overview = overview; // possible data race ?? ? ? ? ?
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
void	set_up(t_overview *overview, t_philosopher *philos)
{
	int	i;

	i = 0;
	(void)philos;
	pthread_mutex_init(&(overview->write_mutex), NULL); 	// err
	pthread_mutex_init(&(overview->done_mutex), NULL); 	// err
	pthread_mutex_lock(&overview->done_mutex);
	overview->done = false;
	pthread_mutex_unlock(&overview->done_mutex);

	// set up watcher thread
	// pthread_create(&(overview->watcher), NULL, watcher_function, &overview);
	// pthread_create(&(overview->watcher), NULL, watcher_function, philos);
	// err

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
	int	i;
	
	pthread_mutex_destroy(&(overview->write_mutex));
	pthread_mutex_destroy(&(overview->done_mutex));
	// other mutexes destroy
	i = 0;
	while (i < overview->philo_count)
	{
		pthread_mutex_destroy(&(overview->forks[i]));
		i++;
	}
	// pthread_join(overview->watcher, NULL); // err
}

void	join_philos(t_philosopher *philos)
{
	int	i;

	i = 0;
	while (i < philos->overview->philo_count)
	{
		pthread_join((philos[i]).philo_thread, NULL);
		// err handling
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_overview		overview;
	t_philosopher	philos[MAX_PHILO_COUNT];

	// printf("going to handle args:\n");
	if (!handle_args(argc, argv, &overview))
		return (EXIT_FAILURE);
	// print_overview(&overview);
	// fail handle ???
	create_philos(&overview, philos);
	set_up(&overview, philos);
	while (check_keep_going(&overview))
	{
		watcher_function(&overview, philos);
	}
	clean_up(&overview);
	join_philos(philos);
	return (EXIT_SUCCESS);
}
