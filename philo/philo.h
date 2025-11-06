/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:40:44 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/06 15:17:03 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdbool.h> // for bool
# include <unistd.h> // for size_t
# define MSG_FORK "has taken a fork\n"
# define MSG_EAT "is eating\n"
# define MSG_SLEEP "is sleeping\n"
# define MSG_THINK "is thinking\n"
# define MSG_DIE "died\n"
# define MSG_INVALID_INPUT "Make sure all inputs are valid positive ints\n"

// large struct for all relevant infooo
typedef struct s_overview
{
	int		philo_count;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		amount_to_eat;
	bool	end_point_given; // set to false if not proivded by user
	int		start_time_msec;
	bool	someone_died;
	// t_philosopher	*philosophers;

}	t_overview;

// every philo has one
typedef struct s_philosopher
{
	t_overview		overview;
	int				philo_nbr;
	unsigned int	recent_meal;
	bool			alive; // set to true at start
}	t_philosopher;


// utils
size_t	ft_strlen(const char *s);
int		philo_atoi(const char *nptr);
unsigned int	get_time_msec(void);

#endif