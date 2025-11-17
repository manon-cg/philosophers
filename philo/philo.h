/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:40:44 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/17 16:21:42 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdbool.h> // for bool
# include <pthread.h> // for pthread_mutex_t
# define MAX_PHILO_COUNT 200
# define MSG_FORK "has taken a fork\n"
# define MSG_EAT "is eating\n"
# define MSG_SLEEP "is sleeping\n"
# define MSG_THINK "is thinking\n"
# define MSG_DIE "died 💀💀💀💀💀💀💀\n"
# define MSG_INVALID_INTS "Make sure all inputs are valid positive ints.\n"
# define MSG_ARG_STRUCTURE "Make sure to structure your input as \
follows:\n./philo [number_of_philosophers] [time_to_die] [time_to_eat] \
[time_to_sleep] [OPTIONAL: number_of_times_each_philosopher_must_eat]\n"

typedef struct s_overview
{
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				amount_to_eat;
	bool			max_food_arg_given;
	int				start_time_msec;
	bool			done;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	done_mutex;
	pthread_mutex_t	fork_mutexes[MAX_PHILO_COUNT];
	int				forks[MAX_PHILO_COUNT];
	pthread_mutex_t	start_mutex;
}	t_overview;

typedef struct s_philosopher
{
	t_overview		*overview;
	int				philo_nbr;
	unsigned int	recent_meal;
	bool			alive; // set to true at start
	pthread_t		philo_thread;
	pthread_mutex_t	recent_meal_lock;
	pthread_mutex_t	hunger_lock;
	int				meal_count;
	bool			hungry;
}	t_philosopher;

enum	e_availability
{
	AVAILABLE,
	TAKEN	
};

enum	e_error
{
	FAIL_INIT_FORK_MUTEX,
	FAIL_INIT_HUNGER_MUTEX,
	FAIL_INIT_RECENT_MEAL_MUTEX,
	FAIL_THREAD
};

// init.c
bool			handle_args(int argc, char *argv[], t_overview *overview);
bool			init_philo_info(t_overview *overview, t_philosopher *philos);
bool			create_philos(t_overview *overview, t_philosopher *philos);
bool			set_up(t_overview *overview);
bool			init_barrier(t_overview *overview);

// actions
void			philo_sleep(t_overview *overview, t_philosopher *philo);
bool			philo_eat(t_overview *overview, t_philosopher *philo);
bool			philo_grab(t_overview *overview, t_philosopher *philo);
void			philo_think(t_overview *overview, t_philosopher *philo);
void			*philo_die(t_overview *overview, t_philosopher *philo);

// fork_utils.c
void			drop_single_fork(t_overview *overview, int fork_nbr);
void			take_fork(t_overview *overview, int fork_nbr);
void			drop_both_forks(t_overview *overview, t_philosopher *philo);

// utils
size_t			ft_strlen(const char *s);
int				philo_atoi(const char *nptr);
unsigned int	get_time_msec(void);
void			print_with_lock(t_overview *overview, t_philosopher *philo, char *msg);
void			print_error(char *message);

// continue_checks.c
bool			philo_full_check(t_philosopher *philo);
bool			philo_death_check(t_philosopher *philo);
bool			check_keep_going(t_overview *overview);
bool			all_full_check(t_philosopher *philos, t_overview *overview);

// cleanup.c
bool			clean_philo_mutex_fail(t_philosopher *philos, int i, int error);
bool			clean_fork_init_fail(t_overview *overview, int i);
void			join_philos(t_philosopher *philos);
void			clean_up(t_overview *overview, t_philosopher *philos,
					bool philos_init);

// main.c
void			*philo_function(void *arg);

// dont handi n
void 	print_overview(t_overview *overview);


#endif