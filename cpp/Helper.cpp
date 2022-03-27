/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Helper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crendeha <crendeha@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 06:08:02 by zefir             #+#    #+#             */
/*   Updated: 2022/03/21 11:34:10 by crendeha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hpp/Helper.hpp"

std::vector<std::string> Helper::ft_split(const std::string& line, char c) {
	std::vector<std::string>	result;
	size_t 						i = 0;
	size_t 						j = 0;
    
	while (line[i] != '\0') {
		if (line[i] == c) {
			result.push_back(line.substr(j, i - j));
			j = i + 1;
		}
		i++;
	}
    result.push_back(line.substr(j, i - j));
	return (result);
}

std::vector<std::string> Helper::ft_splitWS(const std::string& line) {
    std::vector<std::string>	answer;
    size_t 						i = 0;
    size_t 						j = 0;

    while (i < line.size() && j < line.size()) {
        while (i < line.size() && (line[i] == ' ' || line[i] == '\t'))
            i++;
        if (i == line.size())
            break ;
        j = i + 1;
        while (j < line.size() && (line[j] != ' ' && line[j] != '\t'))
            j++;
        answer.push_back(line.substr(i, j - i));
        i = j;
    }
    return answer;
}

std::string Helper::mergeUrls(std::string path1, std::string path2) {
		path1 = path1[path1.size() - 1] == '/' ? path1.substr(0, path1.size() - 1) : path1;
		path2 = path2[0] == '/' ? path2.substr(1, path2.size()) : path2;
		return (std::string(path1 + "/" + path2));
}

bool	Helper::checkValidDir(const char* path) {
	DIR *dir = opendir(path);

	if (dir) {
		closedir(dir);
		return (true);
	}

	return (false);
}

bool	Helper::checkValidFile(const std::string& path) {
	struct stat checkFile;
	return (stat(path.c_str(), &checkFile) == 0 && (checkFile.st_mode & S_IFREG));
}

std::string Helper::decode(std::string &str) {
	int val = 0;
	int valb = -8;
	unsigned char c;
	std::string res;
	std::vector<int> tab(256, -1);

	for (int i = 0; i < 64; i++)
		tab["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i; 
	for (std::string::iterator it = str.begin(); it != str.end(); it++) {
		c = *it;
		if (tab[c] == -1) break;
		val = (val << 6) + tab[c];
		valb += 6;
		if (valb >= 0) {
			res.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return (res);
}

std::string		Helper::getFileExtension(const std::string& str) {
	if(str.find(".") == std::string::npos) return ("");		
	return (str.substr(str.find("."), str.length()));
}

std::string	Helper::readFile(const std::string& path) {
	std::ifstream		fd(path.c_str());
	std::stringstream	buffer;

	if (fd.is_open()) {
		buffer << fd.rdbuf();
		fd.close();
	}
	return (buffer.str());
}

void Helper::strcut(std::string *cut, std::string *src, std::string sep, size_t *index) {
	*index = src->find(sep);
	*src = src->substr(*index + sep.size(), src->size());
	*cut = src->substr(0, *index);
}

std::string Helper::getFileName(const std::string& str) {
	// filename="xxx"
	std::string target = "filename";
	size_t startIdx = str.find(target, 0);
	size_t endIdx = str.find("\"", startIdx + target.size() + 2);
	size_t len = endIdx - (startIdx + target.size() + 2);

	return str.substr(startIdx + target.size() + 2, len);
}

std::string Helper::getFileContent(const std::string& str, size_t content_length) {
	// Parse from body
	std::string sep = "\r\n\r\n";
	size_t startIdx = str.find(sep, 0) + sep.size();
	std::string sep2 = "------";
	size_t endIdx = str.find(sep2, startIdx);
	
	return str.substr(startIdx, endIdx - startIdx - 2);
}

std::string Helper::getFileNameToDelete(const std::string& str) {
	// Parse from URL
	std::string sep = "/";
	size_t startIdx = str.find_last_of(sep) + sep.size();

	return str.substr(startIdx, str.size() - startIdx);
}
