/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:40:44 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/07 17:00:56 by mgroos           ###   ########.fr       */
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
# define MSG_DIE "died\n"
# define MSG_INVALID_INPUT "Make sure all inputs are valid positive ints\n"

// large struct for all relevant infooo
typedef struct s_overview
{
	int				philo_count;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				amount_to_eat;
	bool			end_point_given; // set to false if not proivded by user
	int				start_time_msec;
	bool			someone_died;
	pthread_mutex_t	*write_mutex;
	pthread_mutex_t	*death_mutex;
	// forks ! all of them should have a mutex Somehow. mutex array? :|
	// t_philosopher	*philosophers;

}	t_overview;

// every philo has one
typedef struct s_philosopher
{
	t_overview		*overview;
	int				philo_nbr;
	unsigned int	recent_meal;
	bool			alive; // set to true at start
	pthread_t		*philo_thread;
}	t_philosopher;

// actions
void	philo_sleep(t_overview *overview);
void	philo_eat(t_overview *overview, t_philosopher *philo);
void 	philo_grab(t_overview *overview);
void	philo_think(t_overview *overview);
void	philo_die(t_overview *overview);

// utils
size_t			ft_strlen(const char *s);
int				philo_atoi(const char *nptr);
unsigned int	get_time_msec(void);
void			print_with_lock(t_overview *overview, char *msg);


// dont handi n
void print_overview(t_overview *overview);


#endif