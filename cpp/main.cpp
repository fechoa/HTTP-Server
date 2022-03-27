/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmand <marmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 13:11:01 by zefir             #+#    #+#             */
/*   Updated: 2022/03/19 18:46:10 by marmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hpp/Server.hpp"
#include "../hpp/Res.hpp"
#include "../hpp/Config.hpp"
#include "../hpp/Program.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << __RED__ << "Error! Please try:\n./webserv [PATH TO THE CONFIG]" << __CLEAR__ << std::endl;
        exit(EXIT_FAILURE);
    }
    
    Program program;
    program.initProgram(argv[1]);
	std::cout << __GREEN__ << "Server is listening" << __CLEAR__ << std::endl;
    program.start();

    return (EXIT_SUCCESS);
}
