/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:39:04 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/07 11:11:30 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h> // for printf
#include <stdlib.h> // for malloc/free
#include <stddef.h> // for size_t
#include <pthread.h>  // for threading

void	*philo_function(void *arg)
{
	t_philosopher *philo;
	
	philo = (t_philosopher *)arg;
	while (philo->alive == true)
	{
		printf("running.....\n");
		// eat
		// sleep
		// think
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
 * struct.
 */
bool	handle_args(int argc, char *argv[], t_overview *overview)
{
	if (argc < 5 || argc > 6)
		return (arg_error(), false);
	overview->philo_count = philo_atoi(argv[1]);
	if (overview->philo_count <= 0)
		return (free(overview), printf(MSG_INVALID_INPUT), false);
	// if philo count is more than 200, don't allow
	overview->time_to_die = philo_atoi(argv[2]);
	if (overview->time_to_die <= 0)
		return (free(overview), printf(MSG_INVALID_INPUT), false);
	overview->time_to_eat = philo_atoi(argv[3]);
	if (overview->time_to_eat <= 0)
		return (free(overview), printf(MSG_INVALID_INPUT), false);
	overview->time_to_sleep = philo_atoi(argv[4]);
	if (overview->time_to_sleep <= 0)
		return (free(overview), printf(MSG_INVALID_INPUT), false);
	overview->amount_to_eat = 0;
	overview->end_point_given = false;	
	if (argc > 5)
	{
		overview->amount_to_eat = philo_atoi(argv[5]);
		if (overview->amount_to_eat < 0)
			return (free(overview), printf(MSG_INVALID_INPUT), false);
		overview->end_point_given = true;
	}
	return (true);
}

/** Create the threads to make  */
void	create_philos(t_overview *overview)
{
	int	i;
	pthread_t thread;

	i = 0;
	// while (i < overview->philo_count)
	// {
	// 	pthread_create(&thread, NULL, philo_function, NULL);
	// 	i++;
	// }
	(void)overview;
	printf("creating a thread\n");
	pthread_create(&thread, NULL, philo_function, NULL);
}

/** Set up the mutexes. */
void	set_up(t_overview *overview)
{
	pthread_mutex_init(overview->write_mutex, NULL);
}

/** Clean up the mutexes. */
void	clean_up(t_overview *overview)
{
	pthread_mutex_destroy(overview->write_mutex);
	// other mutexes destroy

}

int	main(int argc, char *argv[])
{
	t_overview		overview;

	// printf("going to handle args:\n");
	if (handle_args(argc, argv, &overview))
	{
		
		// if amount to eat = 0 -> done ? no ?
	}
	overview.start_time_msec = get_time_msec();
	print_overview(&overview);
	// fail handle ?? ?
	create_philos(&overview);
	set_up(&overview);

	printf("TIME in msec?? since when???: %d\n", overview.start_time_msec);
	clean_up(&overview);
}
