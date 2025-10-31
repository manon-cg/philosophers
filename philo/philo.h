/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:40:44 by mgroos            #+#    #+#             */
/*   Updated: 2025/10/31 10:42:41 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdbool.h> // for bool
#include <unistd.h> // for size_t

// large struct for all relevant infooo
typedef struct s_overview
{
	int		philo_count;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		amount_to_eat;
	bool	end_point_given; // set to false if not proivded by user
	
}	t_overview;

// utils
size_t	ft_strlen(const char *s);
int		philo_atoi(const char *nptr, int *valid);

#endif