/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Helper.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crendeha <crendeha@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 06:04:29 by zefir             #+#    #+#             */
/*   Updated: 2022/03/20 16:30:56 by crendeha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPER_HPP
# define HELPER_HPP
# include <string>
# include <vector>
# include <iostream>
# include <dirent.h>
# include <fstream>
# include <sstream>
# include <sys/stat.h>
# include <limits.h>

# define __RED__ "\e[1;31m"
# define __GREEN__   "\033[32m"
# define __CLEAR__ "\e[1;0m"
# define __BUFFER__ 1000000
# define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

struct Helper {
	static std::vector<std::string> ft_split(const std::string& line, char c);
	static std::vector<std::string>	ft_splitWS(const std::string& line);
	static std::string mergeUrls(std::string path1, std::string path2);
	static bool	checkValidDir(const char* path);
	static bool	checkValidFile(const std::string& path);
	static std::string decode(std::string &str);
	static std::string	getFileExtension(const std::string& str);
	static std::string	readFile(const std::string& path);
	static void strcut(std::string *line, std::string *src, std::string sep, size_t *index);
    static std::string getFileName(const std::string& str);
    static std::string getFileContent(const std::string& str, size_t content_length);
    static std::string getFileNameToDelete(const std::string& str);
};

struct ConfigFields
{
    bool    isServer;
    bool    isLocation;
    bool    isRootLocation;
    bool    isServerName;
    bool    isListen;
    bool    isEndField;
    bool    isMethods;
    bool    isRoot;
    bool    isAutoindex;
    bool    isAuth;
    bool    isIndex;
    bool    isCgiPath;
    bool    isCgiExtension;
    bool    isMaxBody;
	bool	isExt;
};

#endif
