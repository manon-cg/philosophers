/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:40:44 by mgroos            #+#    #+#             */
/*   Updated: 2025/09/05 15:54:00 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h> // for bool

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