/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanchev <mdanchev@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 09:44:09 by mdanchev          #+#    #+#             */
/*   Updated: 2024/01/24 09:44:10 by mdanchev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

// GRAPHIC MODES
# define DIM				"\033[1;2m"
# define UNDERLINE			"\033[4m"
# define INVERSE			"\033[7m"

# define ITALIC				"\033[3m"
# define BOLD				"\033[1m"

// RESET
# define END				"\033[0m"

// FORGROUND COLORS
# define BLACK			"\033[30m"
# define RED			"\033[31m"
# define GREEN			"\033[32m"
# define YELLOW			"\033[33m"
# define BLUE			"\033[34m"
# define MAGNETA		"\033[35m"
# define CYAN			"\033[36m"
# define WHITE			"\033[37m"

// FORGROUND BOLD COLORS
# define BLACK_BOLD			"\033[1;30m"
# define RED_BOLD			"\033[1;31m"
# define GREEN_BOLD			"\033[1;32m"
# define YELLOW_BOLD		"\033[1;33m"
# define BLUE_BOLD			"\033[1;34m"
# define MAGNETA_BOLD		"\033[1;35m"
# define CYAN_BOLD			"\033[1;36m"
# define WHITE_BOLD			"\033[1;37m"

// FORGROUND UNDERLINE COLORS
# define BLACK_UL			"\033[4;30m"
# define RED_UL				"\033[4;31m"
# define GREEN_UL			"\033[4;32m"
# define YELLOW_UL			"\033[4;33m"
# define BLUE_UL			"\033[4;34m"
# define MAGNETA_UL			"\033[4;35m"
# define CYAN_UL			"\033[4;36m"
# define WHITE_UL			"\033[4;37m"

// BACKGROUND COLORS
# define BLACK_BG		"\033[40m"
# define RED_BG			"\033[41m"
# define GREEN_BG		"\033[42m"
# define DARK_GREEN_BG		"\x1b[48;5;106m"
# define YELLOW_BG		"\033[43m"
# define BLUE_BG		"\033[44m"
# define MAGNETA_BG		"\033[45m"
# define CYAN_BG		"\033[46m"
# define WHITE_BG		"\033[47m"

#endif
