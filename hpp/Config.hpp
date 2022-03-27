/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zefir <zefir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:10:05 by zefir             #+#    #+#             */
/*   Updated: 2022/03/17 22:11:12 by zefir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <fstream>
# include <sstream>
# include <map>
# include <iostream>
# include <string>
# include "Loc.hpp"
# include "Server.hpp"

class Req;
class Server;

class Config {
private:
	std::ifstream						fd;
	std::map<std::string, std::string>  config;
	std::string							listen;
    std::vector<std::string>            listenList;
	std::vector<Loc>					locationList;
	std::vector<Loc>					locationListExt;	
	std::vector<Server>					serverList;

public:

	Config();
	Config(const Config& cp);
	~Config();
	Config& operator=(const Config& cp);

	bool	checkRepeatListen(const std::string& listen);
	void	confInit();
	void	clearLists(bool isServerToo);
	void	parsing(const std::string& configFile);
	void	validExtensionConfig(const std::string& configFile);
	void	processParsingConfig();
	void	parseLocation(const std::string& location,
						std::vector<std::string>::iterator& begin,
						std::vector<std::string>::iterator& end);

	std::vector<Server>	getServers() const;	
};

#endif
