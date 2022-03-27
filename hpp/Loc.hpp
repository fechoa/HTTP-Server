/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zefir <zefir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:59:00 by zefir             #+#    #+#             */
/*   Updated: 2022/03/17 22:11:04 by zefir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOC_HPP
# define LOC_HPP

# include <string>
# include <vector>
# include <map>
# include <iostream>
# include "Helper.hpp"

class Loc {
private:
	std::vector<std::string>			pages;
	std::vector<std::string>			methods;
	std::map<std::string, std::string>	errors;
	std::map<std::string, std::string>	locations;
	
	std::string							listenStr;
	std::string							path;
	std::string							serverName;
	std::string							clientMaxBodySizeStr;
	size_t								clientMaxBodySize;
	std::string							root;
	std::string							cgiPath;
	std::string							cgiExt;
	std::string							autoindex;
	std::string							authentication;
	bool								isset;

public:
	Loc();
	Loc(const std::map<std::string, std::string>& locationMap);
	Loc(const Loc& cp);
	~Loc();
	Loc& operator=(const Loc& cp);

	std::string							getListen() const;
	std::string							getPath() const;
	std::map<std::string, std::string>	getLocations() const;
	std::string							getServerName() const;
	size_t								getClientMaxBodySize() const;
	std::string							getClientMaxBodySizeStr() const;
	std::string							getRoot() const;
	std::string							getCgiPath() const;
	std::string							getCgiExt() const;
	std::string							getAutoindex() const;
	std::string							getAuthentication() const;
	std::vector<std::string>			getIndex() const;
	std::vector<std::string>			getAcceptedMethod() const;
	std::map<std::string, std::string>	getErrorPage() const;
	std::map<std::string, std::string>	getSettingMap() const;
	bool								isSet() const;
	void								print() const;
};

#endif