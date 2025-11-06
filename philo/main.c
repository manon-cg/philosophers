/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:39:04 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/06 15:16:02 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h> // for printf
#include <stdlib.h> // for malloc/free
#include <stddef.h> // for size_t
#include <pthread.h>  // for threading

void	*philo_function(void *arg)
{
	// (t_overview *)arg;
	// while (arg->someone_died == false)
	// {
	// 	printf("running.....\n");
	// 	// eat
	// 	// sleep
	// 	// think
	// 	usleep(1000);
	// }
	(void)arg;
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

/** For testing only: prints philo struct. */
void print_overview(t_overview *overview)
{
	printf("philocount: %i\ntime_to_die: %i\ntime_to_eat: %i\n", 
		overview->philo_count, overview->time_to_die, overview->time_to_eat);
	printf("time_to_sleep: %i\namount_to_eat: %i\n", overview->time_to_sleep,
		overview->amount_to_eat);
}

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

int	main(int argc, char *argv[])
{
	t_overview		overview;
	pthread_mutex_t	mutex;

	// printf("going to handle args:\n");
	if (handle_args(argc, argv, &overview))
	{
		print_overview(&overview);
		// if amount to eat = 0 -> done ? no ?
	}
	overview.start_time_msec = get_time_msec();
	// fail handle ?? ?
	create_philos(&overview);
	pthread_mutex_init(&mutex, NULL);

	pthread_mutex_destroy(&mutex);

	printf("TIME in msec?? since when???: %d\n", overview.start_time_msec);
}
