/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmand <marmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 15:57:24 by zefir             #+#    #+#             */
/*   Updated: 2022/03/19 20:51:39 by marmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hpp/Config.hpp"

Config::Config() {
	confInit();
}
Config::~Config() {}

Config::Config(const Config& cp) {
	*this = cp;
}

void Config::confInit() {
	config["server_name"] = "";
	config["listen"] = "127.0.0.1:8080";
	config["client_max_body_size"] = "";
	config["accepted_method"] = "";
	config["autoindex"] = "off";        
	config["index"] = "";
	config["cgi_ext"] = "";
	config["cgi_path"] = "";
	config["authentication"] = "";
	config["root"] = "";
}

Config& Config::operator=(const Config& cp) {
	if (this != &cp) {
		config = cp.config;
		listenList = cp.listenList;
		locationList = cp.locationList;
		locationListExt = cp.locationListExt;
	}
	return (*this);
}

bool Config::checkRepeatListen(const std::string& listen) {
	std::vector<std::string>::iterator it = listenList.begin();
	while (it != listenList.end() && *it != listen) it++;
	return it != listenList.end();
}

void	Config::clearLists(bool isServerToo) {
		config.clear();
		locationList.clear();
		locationListExt.clear();
		if (isServerToo == true) serverList.clear();
}

void	Config::parsing(const std::string& configFile) {
	validExtensionConfig(configFile);
	processParsingConfig();
}

void	Config::validExtensionConfig(const std::string& configFile) {
	int checkPos = configFile.find(".conf") == std::string::npos ? -1 : 1;

	if (checkPos == -1 || configFile.size() == 5) exit(EXIT_FAILURE); // Error extension
	fd.open(configFile);
	if (fd.fail()) exit(EXIT_FAILURE); // Error open
}

void	Config::processParsingConfig() {
	std::string text;
	std::getline(fd, text, '\0');
	std::vector<std::string> texts = Helper::ft_split(text, '\n');
	std::vector<std::string> row;
	ConfigFields    checkConf;
	std::map<std::string, std::string> tmpLocation;
	memset(&checkConf, 0, sizeof(ConfigFields));
	tmpLocation["path"] = "/";

	std::vector<std::string>::iterator it = texts.begin();
	while (it != texts.end()) {
		row = Helper::ft_splitWS(*it);

		if (row.size() == 0 || row[0][0] == '#');
		else if (checkConf.isServer == false && row[0].compare("server") == 0 && row[1].compare("{") == 0 && row.size() == 2) {
			memset(&checkConf, 0, sizeof(ConfigFields));
			checkConf.isServer = true;
		}
		else if (checkConf.isServer == true && row[0].compare("listen") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isListen == true || checkRepeatListen(row[1].substr(0, row[1].size() - 1)) == true) exit(EXIT_FAILURE); // Error duplicate	
			checkConf.isListen = true;
			config["listen"] = row[1].substr(0, row[1].size() - 1);
			listenList.push_back(config["listen"]);
			listen = config["listen"];
		}
		else if (checkConf.isServer == true && row[0].compare("server_name") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isServerName == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isServerName = true;
			config["server_name"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (checkConf.isServer == true && row[0].compare("accepted_method") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isMethods == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isMethods = true;
			config["accepted_method"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (checkConf.isServer == true && row[0].compare("client_max_body_size") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isMaxBody == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isMaxBody = true;
			config["client_max_body_size"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (checkConf.isServer == true && row[0].compare("root") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isRoot == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isRoot = true;
			config["root"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (checkConf.isServer == true && row[0].compare("autoindex") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isAutoindex == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isAutoindex = true;
			config["autoindex"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (checkConf.isServer == true && row[0].compare("authentication") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isAuth == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isAuth = true;
			config["authentication"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (checkConf.isServer == true && row[0].compare("index") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isIndex == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isIndex = true;
			config["index"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (checkConf.isServer == true && row[0].compare("cgi_path") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isCgiPath == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isCgiPath = true;
			config["cgi_path"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (checkConf.isServer == true && row[0].compare("cgi_ext") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isCgiExtension == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isCgiExtension = true;
			config["cgi_ext"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (checkConf.isServer == true && row[0].compare("error_page") == 0 && row[2].find(";") == row[2].size() - 1 && row.size() == 3) {
			if (config.count(row[1]) != 0) exit(EXIT_FAILURE); // Error duplicate
			config[row[1]] = row[2].substr(0, row[2].size() - 1);
		}
		else if (checkConf.isServer == true && row.size() == 3 && row[0].compare("location") == 0 && row[2].compare("{") == 0) {
			if (row[1].compare("/") == 0) checkConf.isRootLocation = true;
			checkConf.isLocation = true;
			std::vector<std::string>::iterator end = texts.end();
			parseLocation(row[1], it, end);
		}
		else if (checkConf.isServer == true && row[0].compare("}") == 0 && row.size() == 1) {
			if (checkConf.isListen == false) exit(EXIT_FAILURE); // Error syntax
			checkConf.isServer = false;
			checkConf.isEndField = true;
			if (checkConf.isRootLocation == false) {
				std::map<std::string, std::string>::iterator it = config.begin();
				while (it != config.end()) {
					if (!tmpLocation.count(it->first)) tmpLocation[it->first] = it->second;
					it++;
				}
				locationList.push_back(Loc(tmpLocation));
			}
			checkConf.isRootLocation = false;
			serverList.push_back(Server(listen, locationList, locationListExt));
			tmpLocation.clear();
			tmpLocation["path"] = "/";
			clearLists(false);
			confInit();
		}
		else exit(EXIT_FAILURE); // Error syntax
		it++;
		row.clear();
	}

	if ((checkConf.isServer == true && checkConf.isEndField == false)
		|| checkConf.isLocation == false || checkConf.isServerName == false)
		exit(EXIT_FAILURE); // Error isn't enough info
}

void	Config::parseLocation(const std::string& location,
						std::vector<std::string>::iterator& begin,
						std::vector<std::string>::iterator& end) {

	std::map<std::string, std::string>	tmpLocation;
	std::vector<std::string> row;
	ConfigFields    checkConf;
	memset(&checkConf, 0, sizeof(ConfigFields));
	tmpLocation["path"] = location[0] == '*' && location[1] == '.' ? location.substr(1) : location;			
	checkConf.isExt = tmpLocation["path"] == location ? false : true;
	begin++;

	while (begin != end) {
		row = Helper::ft_splitWS(*begin);
	
		if (row.size() == 0 || row[0][0] == '#');
		else if (row[0].compare("accepted_method") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isMethods == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isMethods = true;
			tmpLocation["accepted_method"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (row[0].compare("root") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isRoot == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isRoot = true;
			tmpLocation["root"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (row[0].compare("autoindex") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isAutoindex == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isAutoindex = true;
			tmpLocation["autoindex"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (row[0].compare("authentication") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isAuth == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isAuth = true;
			tmpLocation["authentication"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (row[0].compare("index") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isIndex == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isIndex = true;
			tmpLocation["index"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (row[0].compare("cgi_path") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isCgiPath == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isCgiPath = true;
			tmpLocation["cgi_path"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (row[0].compare("cgi_ext") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isCgiExtension == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isCgiExtension = true;
			tmpLocation["cgi_ext"] = row[1].substr(0, row[1].size() - 1);
		}
		else if (row[0].compare("client_max_body_size") == 0 && row[1].find(";") == row[1].size() - 1 && row.size() == 2) {
			if (checkConf.isMaxBody == true) exit(EXIT_FAILURE); // Error duplicate
			checkConf.isMaxBody = true;
			tmpLocation["client_max_body_size"] = row[1].substr(0, row[1].size() - 1);						
		}
		else if (row.size() == 3 && row[0].compare("error_page") == 0 && row[2].find(";") == row[2].size() - 1) {
			if (tmpLocation.count(row[1])) exit(EXIT_FAILURE); // Error duplicate
			tmpLocation[row[1]] = row[2].substr(0, row[2].size() - 1);
		}
		else if (row[0].compare("}") == 0 && row.size() == 1) {
			checkConf.isEndField = true;
			if (checkConf.isExt == false) {
				std::map<std::string, std::string>::iterator it = config.begin();
				while (it != config.end()) {
					if (!tmpLocation.count(it->first)) tmpLocation[it->first] = it->second;
					it++;
				}
				locationList.push_back(Loc(tmpLocation));
			}
			else if (checkConf.isExt == true) {
				tmpLocation["cgi_ext"] = tmpLocation["path"];
				locationListExt.push_back(Loc(tmpLocation));
			}
			break ;
		}			
		else
			exit(EXIT_FAILURE); // Error syntax
		
		begin++;
		row.clear();
	}

	if (checkConf.isEndField == false)
		exit(EXIT_FAILURE); // Error syntax
}

std::vector<Server>	Config::getServers() const {
	return (serverList);
}
