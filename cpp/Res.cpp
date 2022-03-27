/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Res.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmand <marmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:02:45 by zefir             #+#    #+#             */
/*   Updated: 2022/03/21 10:37:34 by marmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hpp/Res.hpp"

Res::~Res() {}

Res::Res(const Res& cp) {
	*this = cp;
}

Res::Res(Req* req, int socket)
: req(req), sock(socket), loc(req->getPickLoc()), versionHTTP("HTTP/1.1"), isAuth(false), setFail(false) {
	resMsgs[200] = "OK";						
	resMsgs[201] = "CREATED";
	resMsgs[202] = "ACCEPTED";					
	resMsgs[204] = "NO_CONTENT";				
	resMsgs[400] = "BAD_Req";					
	resMsgs[401] = "UNAUTHORIZED";			
	resMsgs[403] = "FORBIDDEN";					
	resMsgs[404] = "FILE_NOT_FOUND";				
	resMsgs[405] = "METHOD_NOT_ALLOWED";			
	resMsgs[413] = "PAYLOAD_TOO_LARGE";		
	resMsgs[500] = "INTERNAL_ERROR";				
	resMsgs[505] = "HTTP_VERSION_NOT_SUPPORTED";	
	stat(req->getUrlAbsolute().c_str(), &pointStat);
	createResponse();
}

void	Res::createResponse() {
	if (req->getIsBadReq()) setError(400);
	else if (req->getVersHttp() != "HTTP/1.1") setError(505);
	else if (isAuthValid() == false) setError(401);
	else if (!isValidMethod()) setError(405);
	else if (req->getContentLength() > loc.getClientMaxBodySize()) setError(413);
	else if (req->getMethod() == "GET") processGet();
	else if (req->getMethod() == "POST") processPost();
	else if (req->getMethod() == "DELETE") processDelete();
	
	createHeader();
}

void	Res::createHeader() {
	std::ostringstream head;

	head << versionHTTP << " " << resCode << " " << resMsgs[resCode] << "\r\n";

	headers["Content-Type"] = contentType;
	headers["Content-Length"] = SSTR(body.size());
	headers["Host"] = req->getPickLoc().getListen();
	headers["Server"] = req->getPickLoc().getServerName();

	if (resCode == 401) headers["WWW-Authenticate"] = "Basic realm=\"acces to webserv\"";
	if (resCode == 201) headers["Location"] = req->getUrl();
	else headers["Content-Location"] = req->getUrl();

	std::map<std::string, std::string> tmpHeaders = headers;
	std::map<std::string, std::string> tmpCgiHeaders = headersCgi;

	std::map<std::string, std::string>::iterator it = tmpCgiHeaders.begin();
	while (it != tmpCgiHeaders.end()){
		if (it->first != "Status") tmpHeaders[it->first] = it->second;
		it++;
	}
	it = tmpHeaders.begin();
	while (it != tmpHeaders.end()) {
		if (!it->second.empty())	head << it->first << ": " << it->second << "\r\n";
		it++;
	}

	header = head.str() + "\r\n";
	if (not isCGIres)
		res = header + body;
}

void	Res::processGet() {
	if (Helper::checkValidDir(req->getUrlAbsolute().c_str())) {
		if (isHaveIndexPage(req->getUrlAbsolute())) req->pointUp(getIndexPage());
		else if(loc.getAutoindex() == "on" && fillAutoIndex()) return;
	}
	isCGIres = false;
	if (!loc.getCgiPath().empty()) { // && (loc.getCgiExt() == Helper::getFileExtension(req->getPoint())
		Cgi	cgi(req, this);

		if (cgi.actions()) {
			res = cgi.getResult();
			isCGIres = true;
			setBody(cgi.getResult());
			headersCgi = cgi.getCgiHeaders();
			if (headersCgi["Status"] != "") {
				std::string statusCode = headersCgi["Status"].substr(0, headersCgi["Status"].find(" "));
				setResCode(atoi(statusCode.c_str()));
			}
			else setResCode(200);
		}
		else setError(500);
	}
	else langCheck();

	getLastChange();
	setContentType(getContentType(req->getPoint()));
}

void Res::processPost() {
	std::string file_name = Helper::getFileName(req->getBody());
	std::string path = "./storage/files/" + file_name;
	std::ofstream out(path);
	if (out.is_open()) {
		out << Helper::getFileContent(req->getBody(), req->getContentLength());
		setResCode(200);
	} else {
		setError(400);
	}
	out.close();
}

void	Res::processDelete() {
	std::string file_name = Helper::getFileNameToDelete(req->getUrlAbsolute());
	std::string path = "./storage/files/" + file_name;
	std::ifstream is(path);

	if (is.is_open()) {
		is.close();
		if (std::remove(path.c_str()) == 0) {
			setResCode(204);
		} else {
			setResCode(202);
		}
	} else {
		setError(404);
	}
}

void	Res::langCheck() {
	if (req->getHeaders()["Accept-Language"] != "") {
		std::vector<std::string> lang = Helper::ft_split(req->getHeaders()["Accept-Language"], ',');
		std::vector<std::string>::iterator it = lang.begin();
		
		while (it != lang.end()) {
			if (Helper::checkValidFile(req->getUrlAbsolute() + "." + it->substr(0,2))) {
				headers["Content-Language"] = it->substr(0,2);;
				setResCode(200);
				setBody(Helper::readFile(req->getUrlAbsolute() + "." + it->substr(0,2)));
				return ;
			}
			it++;
		}
	}
	if (Helper::checkValidFile(req->getUrlAbsolute())) {
		setResCode(200);
		setBody(Helper::readFile(req->getUrlAbsolute()));
	}
	else setError(404);
}

bool	Res::fillAutoIndex() {
	std::ostringstream box;
	DIR				*dir;
	struct dirent	*dirList;

	if ((dir = opendir(req->getUrlAbsolute().c_str()))) {
		setContentType("text/html");
		setResCode(200);

		box << Helper::readFile("storage/auto/header.html");
		box << "<ul class=\"list\">" << std::endl;
		while((dirList = readdir(dir))) {
			box << "<li>" << "<a href='" << Helper::mergeUrls(req->getUrl(), dirList->d_name) <<
			"'>" << dirList->d_name << "</a>" << "</li>" << std::endl;
		}
		box << "</ul>" << std::endl;
		box <<  Helper::readFile("storage/auto/footer.html");;
		setBody(box.str());
		closedir(dir);
		return (true);
	}
	return (false);
}

void	Res::setError(int num) {
	std::string	numStr = SSTR(num);

	setResCode(num);
	setContentType("text/html");
	if(!loc.getErrorPage()[numStr].empty()) setBody(Helper::readFile(loc.getErrorPage()[numStr]));
	else setBody(addDefError(numStr, resMsgs[num]));
	headers["Retry-After"] = "10";
	setFail = true;
}

std::string		Res::addDefError(const std::string& code, const std::string& msg) {
	std::ostringstream	bd;

	bd << Helper::readFile("storage/error/1.html");
	bd << "error " << code;
	bd << Helper::readFile("storage/error/2.html");
	bd << "<h1>" << code << "</h1>";
	bd << "<p>" << msg << "</p>";
	bd << Helper::readFile("storage/error/3.html");

	return (bd.str());
}

void	Res::getLastChange() {
	char time[40];
	memset(time, 0, 40);
	strftime(time, 40, "%a, %d %b %Y %X %Z", localtime(&(pointStat.st_ctime)));
	headers["Last-Modified"] = time;
}

bool	Res::isAuthValid() {
	std::vector<std::string>	words;
	std::string					authen(loc.getAuthentication());
	std::string					authoriz(req->getHeaders()["Authorization"]);

	if (authen.empty()) return (true);
	if (authoriz.empty()) return (false);

	words = Helper::ft_splitWS(authoriz);

	if (words[0].compare("Basic")) return (false);

	authoriz = Helper::decode(words[1]);

	if (authen.compare(authoriz)) return (false);

	isAuth = true;
	return (true);
}

bool	Res::isValidMethod() {
	std::vector<std::string>	meths = loc.getAcceptedMethod();
	if (!isValidHttpMethod()) return (false);
	if (meths.empty()) return (true);
	std::vector<std::string>::iterator it = meths.begin();
	while(it != meths.end()) {
		if (req->getMethod() == *it) return (true);
		it++;
	}

	return (false);
}

bool	Res::isValidHttpMethod() {
	std::string listOfvalidHttpMethods[3] = {"GET", "POST", "DELETE"}; 
	for (int i = 0; i < 3; i++)
		if (listOfvalidHttpMethods[i] == req->getMethod()) return (true);
	return (false);
}

bool	Res::isHaveIndexPage(const std::string& urlPathAbsolute) {
	std::vector<std::string> indexPages = loc.getIndex();
	std::vector<std::string>::iterator it = indexPages.begin();
	
	while(it != indexPages.end()) {
		if (*it == "") return (false);
		std::string checkopen(Helper::mergeUrls(urlPathAbsolute, *it));
		std::ifstream 	fd(checkopen.c_str());
		if (!fd.fail()) {
			fd.close();
			return (true);
		}
		fd.close();
		it++;
	}
	return (false);
}

std::string	Res::getIndexPage() {
	std::vector<std::string> indexPages = loc.getIndex();
	std::vector<std::string>::iterator it = indexPages.begin();

	if (indexPages.empty()) return(req->getPoint());
	while(it != indexPages.end()) {
		std::string checkopen = Helper::mergeUrls(req->getUrlAbsolute(), *it);
		std::ifstream 	fd(checkopen.c_str());
		if (!fd.fail()) {
			fd.close();
			return (Helper::mergeUrls(req->getPoint(), *it));
		}
		fd.close();
		it++;
	}
	return (req->getPoint());
}

std::string	Res::getContentType(const std::string& point) {
	std::string exts[62] = {"php", "aac", "abw", "arc", "avi", "azw", "bin", "bz", "bz2", "csh", "css", "csv", "doc", "docsx", "eot", "epub", "gif", "htm", "html", "ico",
	"ics", "jar", "jpeg", "jpg", "js", "json", "mid", "midi", "mpeg", "mpkg", "odp", "ods", "odt", "oga", "ogv", "ogx", "otf", "png", "pdf", "ppt", "pptx", "rar", "rtf", "sh"
	"svg", "swf", "tar", "tif", "tiff", "ts", "ttf", "vsd", "wav", "weba", "webm", "webp", "woff" ,"woff2", "xhtml", "xls", "xlsx","xml"};
	std::string types[62] = {"text/html", "audio/aac", "application/x-abiword", "application/octet-stream", "video/x-msvideo", "application/vnd.amazon.ebook", "application/octet-stream", "application/x-bzip",
	"application/x-bzip2", "application/x-csh", "text/css", "text/csv", "application/msword", "application/vnd.openxmlformats-officedocument.wordprocessingml.document", "application/vnd.ms-fontobject",
	"application/epub+zip", "image/gif", "text/html", "text/html", "image/x-icon", "text/calendar", "application/java-archive", "image/jpeg", "image/jpeg" ,"application/javascript", "application/json",
	"audio/midi","audio/midi", "video/mpeg", "	application/vnd.apple.installer+xml", "application/vnd.oasis.opendocument.presentation", "application/vnd.oasis.opendocument.spreadsheet", "application/vnd.oasis.opendocument.text", "audio/ogg", "video/ogg", "application/ogg", "font/otf", "image/png", "application/pdf", "application/vnd.ms-powerpoint", "application/vnd.openxmlformats-officedocument.presentationml.presentation",
	"application/x-rar-compressed", "application/rtf", "application/x-sh", "image/svg+xml", "application/x-shockwave-flash", "application/x-tar", "image/tiff", "image/tiff", "application/typescript", "font/ttf", "application/vnd.visio", "audio/x-wav", "audio/webm", "video/webm", "image/webp", "font/woff", "font/woff2", "application/xhtml+xml", "application/vnd.ms-excel", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
	"application/xml"};

	if (Helper::getFileExtension(point) == "") return ("text/plain");
	for (int j = 0; j < 67; j++)
		if (exts[j] == Helper::getFileExtension(point).substr(1, Helper::getFileExtension(point).size()))
			return (types[j]);
	return ("");
}

void	Res::setResCode(int code) {
	resCode = setFail == false ? code : resCode;
}

void	Res::setBody(const std::string& bd) {
	body = setFail == false ? bd : body;
}

void	Res::setContentType(const std::string& type) {
	contentType = setFail == false ? type : contentType;
}

std::string Res::getHeader() const {
	return (header);
}

std::string Res::getBody() const {
	return (body);
}

std::string Res::getRes() const {
	return (res);
}

std::string		Res::getResCodeStr() const {
	return (SSTR(resCode));
}

int	Res::getResCode() const {
	return(resCode);
}

std::string		Res::getResCodeMessage() {
	return (resMsgs[resCode]);
}

std::string		Res::getAuthorization() const {
	return (authorization);
}

bool	Res::getIsAuth() const {
	return (isAuth);
}

int	Res::getHeaderLength() const {
	return (header.size());
}

int	Res::getBodyLength() const {
	return (body.size());
}
int	Res::getResLength() const {
	return (res.size());
}
