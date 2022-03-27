/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crendeha <crendeha@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 13:07:41 by tpalhol           #+#    #+#             */
/*   Updated: 2022/03/20 18:11:57 by crendeha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <string>
# include <map>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "Req.hpp"
# include "Res.hpp"

class Res;

class Cgi {
private:
	Req*								req;
	Res*								res;
	std::map<std::string, std::string>	env;
	std::string							result;
	std::map<std::string, std::string>	headers;
	char								**envs;
	char								**args;


public:
	Cgi(Req* req, Res* res);
	Cgi(const Cgi& cp);
	~Cgi();

	bool								actions();
	std::string							getResult();
	void								parseResponse();
	std::map<std::string, std::string>	getCgiHeaders();
	std::string							transformEnv(std::string form);
	void								fillEnv();
	void								fillArgs();
	void								freeStrs(char** strs);	
	char								**toArr();	
};

#endif