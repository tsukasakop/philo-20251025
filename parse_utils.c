/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkondo <tkondo@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 09:35:00 by tkondo           #+#    #+#             */
/*   Updated: 2025/01/26 09:35:00 by tkondo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_atoi - Converts a string to an integer
 * @str: String to convert
 *
 * This function converts a string representation of a number to an integer.
 * It handles leading whitespace, optional sign character, and numeric digits.
 * This is a simplified version of the standard atoi function.
 *
 * Return: The converted integer value
 */
int	ft_atoi(const char *str)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

/**
 * is_valid_number - Validates if a string represents a valid number
 * @str: String to validate
 *
 * This function checks if a string contains only valid numeric characters.
 * It allows an optional sign at the beginning but requires at least one
 * digit after the sign. Used for input validation.
 *
 * Return: 1 if the string is a valid number, 0 otherwise
 */
int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
