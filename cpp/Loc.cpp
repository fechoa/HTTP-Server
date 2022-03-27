/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zefir <zefir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 15:00:40 by zefir             #+#    #+#             */
/*   Updated: 2022/03/17 22:59:47 by zefir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hpp/Loc.hpp"

Loc::Loc(): isset(false) {}
Loc::~Loc() {}

Loc::Loc(const Loc& cp) {
	*this = cp;
}

Loc::Loc(const std::map<std::string, std::string>& locationMap)
:  clientMaxBodySizeStr("1M"), clientMaxBodySize(1000000), locations(locationMap), isset(true) {	

	std::map<std::string, std::string>::iterator it = locations.begin();
	while (it != locations.end()) {
		if (it->first.compare("listen") == 0) listenStr = it->second;
		else if (it->first.compare("path") == 0) path = it->second;
		else if (it->first.compare("server_name") == 0) serverName = it->second;
		else if (it->first.compare("client_max_body_size") == 0) {
			clientMaxBodySizeStr = it->second.size() > 0 ? it->second : clientMaxBodySizeStr;
			size_t result = 1;
			if (clientMaxBodySizeStr[clientMaxBodySizeStr.size() - 1] == 'K') result = 1000;
			else if (clientMaxBodySizeStr[clientMaxBodySizeStr.size() - 1] == 'M') result = 1000000;
			else if (clientMaxBodySizeStr[clientMaxBodySizeStr.size() - 1] == 'G') result = 1000000000;
			else if (SSTR(atol(clientMaxBodySizeStr.c_str())) != clientMaxBodySizeStr) exit(EXIT_FAILURE); // Error body size
			clientMaxBodySize = result * atol(clientMaxBodySizeStr.c_str());
		}
		else if (it->first.compare("root") == 0) root = it->second;
		else if (it->first.compare("autoindex") == 0) autoindex = it->second;
		else if (it->first.compare("authentication") == 0) authentication = it->second;
		else if (it->first.compare("cgi_path") == 0) cgiPath = it->second;
		else if (it->first.compare("cgi_ext") == 0) cgiExt = it->second;
		else if (it->first.compare("index") == 0) pages = Helper::ft_split(it->second, '/');
		else if (it->first.compare("accepted_method") == 0) methods = it->second.size() > 0 ? Helper::ft_split(it->second, '/') : methods;
		else errors[it->first] = it->second;
		it++;
	}
}

Loc& Loc::operator=(const Loc& cp) {
	if (this != &cp) {
		listenStr = cp.listenStr;
		path = cp.path;
		serverName = cp.serverName;
		clientMaxBodySize = cp.clientMaxBodySize;
		clientMaxBodySizeStr = cp.clientMaxBodySizeStr;
		root = cp.root;
		cgiPath = cp.cgiPath;
		cgiExt = cp.cgiExt;
		autoindex = cp.autoindex;
		authentication = cp.authentication;
		pages = cp.pages;
		methods = cp.methods;
		errors = cp.errors;
		locations = cp.locations;
	}
	return (*this);
}

std::string	Loc::getListen() const  {
	return (listenStr);
}

std::string	Loc::getPath() const {
	return (path);
}

std::map<std::string, std::string>	Loc::getLocations() const {
	return (locations);
}

std::string	Loc::getServerName() const  {
	return (serverName);
}

size_t	Loc::getClientMaxBodySize() const  {
	return (clientMaxBodySize);
}

std::string	Loc::getClientMaxBodySizeStr() const  {
	return (clientMaxBodySizeStr);
}

std::string	Loc::getRoot() const  {
	return (root);
}

std::string	Loc::getCgiPath() const  {
	return (cgiPath);
}

std::string	Loc::getCgiExt() const  {
	return (cgiExt);
}

std::string	Loc::getAutoindex() const  {
	return (autoindex);
}

std::string	Loc::getAuthentication() const  {
	return (authentication);
}

std::vector<std::string>	Loc::getIndex() const  {
	return (pages);
}

std::vector<std::string>	Loc::getAcceptedMethod() const  {
	return (methods);
}

std::map<std::string, std::string>	Loc::getErrorPage() const  {
	return (errors);
}

std::map<std::string, std::string>	Loc::getSettingMap() const  {
	return (locations);
}

bool	Loc::isSet() const  {
	return (isset);
}

void	Loc::print() const {
	std::cout << "location \n";
	std::cout << "\tpath: " << path << "\n";
	std::cout << "\tlisten: " << listenStr << "\n";
	std::cout << "\tserver_name: " << serverName << "\n";
	std::cout << "\tclient_max_body_size: " << clientMaxBodySizeStr << "[" << clientMaxBodySize << "]\n";
	std::cout << "\troot: " << root << "\n";
	std::cout << "\tautoindex: " << autoindex << "\n";
	std::cout << "\tauthentication: " << authentication << "\n";
	std::cout << "\tcgi_path: " << cgiPath << "\n";
	std::cout << "\tcgi_ext: " << cgiExt << "\n";
	
	std::cout << "\tindex: ";
	std::vector<std::string>::const_iterator	it = pages.begin();
	while (pages.size() && it != pages.end()) {
		std::cout << *it << " | ";
		it++;
	}
	std::cout << "\n";
	
	std::cout << "\taccepted_method: ";
	it = methods.begin();
	while (methods.size() && it != methods.end()) {
		std::cout << *it << " | ";
		it++;
	}
	std::cout << "\n";
	
	std::cout << "\terror_page: ";
	std::map<std::string, std::string>::const_iterator	it2 = errors.begin();
	while (errors.size() && it2 != errors.end()) {
		std::cout << "["  << it2->first << "][" << it2->second << "]   ";
		it2++;
	}
	std::cout << "\n" << std::endl;
}