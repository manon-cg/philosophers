/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:39:04 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/10 15:40:22 by mgroos           ###   ########.fr       */
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

	// printf("running a death check!\n");
	current_time = get_time_msec();
	// erorr handling
	printf("%i: rcurrent: %d // recent meal: %d diff: %d\n", 
philo->philo_nbr, current_time, philo->recent_meal, current_time - philo->recent_meal);
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
	// usleep(1000);
	while (!philo->overview->someone_died)
	{
		if (philo_death_check(philo))
			philo_die(philo->overview, philo);
		if (philo->overview->philo_count % 2 == 0)
		{
			if (philo->philo_nbr % 2 == 0)
				usleep(100);
			// // more chekc
			// if (philo_death_check(philo))
			// 	philo_die(philo->overview, philo);
			// real
			philo_grab(philo->overview, philo);
			philo_eat(philo->overview, philo);
			philo_sleep(philo->overview, philo);
			philo_think(philo->overview, philo);
		}
		else
		{
			philo_grab(philo->overview, philo);
			philo_eat(philo->overview, philo);
			philo_sleep(philo->overview, philo);
			philo_think(philo->overview, philo);
		}
		if (philo_death_check(philo))
			philo_die(philo->overview, philo);
		usleep(1100);
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
		{
			// printf("SOMEONE DIED according to watcher\n");
			pthread_mutex_lock(&overview->write_mutex);
			printf("%d DEATH FOUND BY WATCHER!\n", get_time_msec());
			pthread_mutex_unlock(&overview->write_mutex);
			break ;
		}
		usleep(1000);
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
	overview->start_time_msec = get_time_msec();
	while (i < overview->philo_count)
	{
		(philos[i]).philo_nbr = i + 1;
		(philos[i]).alive = true;
		(philos[i]).overview = overview;
		(philos[i]).meal_count = 0;
		(philos[i]).recent_meal = overview->start_time_msec;
		pthread_mutex_init(&(philos[i]).recent_meal_lock, NULL);
		// err handling
		pthread_create(&(philos[i]).philo_thread, NULL, philo_function, &philos[i]);
		// err handling
		i++;
	}
	// i = 0;
	// while (i < overview->philo_count)
	// {
	// 	(philos[i]).recent_meal = overview->start_time_msec;
	// 	i++;
	// }
}

/** Set up the mutexes. */
void	set_up(t_overview *overview)
{
	int	i;

	i = 0;
	pthread_mutex_init(&(overview->write_mutex), NULL); 	// err
	pthread_mutex_init(&(overview->death_mutex), NULL); 	// err
	overview->someone_died = false;

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
	int	i;
	
	pthread_mutex_destroy(&(overview->write_mutex));
	pthread_mutex_destroy(&(overview->death_mutex));
	// other mutexes destroy
	i = 0;
	while (i < overview->philo_count)
	{
		pthread_mutex_destroy(&(overview->forks[i]));
		i++;
	}
	pthread_join(overview->watcher, NULL); // err
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
	set_up(&overview);
	create_philos(&overview, philos);
	while (!overview.someone_died)
	{
		usleep(1000);
	}
	clean_up(&overview);
	join_philos(philos);
	return (EXIT_SUCCESS);
}
