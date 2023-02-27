#include "../../header/response/directory_index.hpp"

std::string	dir_list(char * relativePath, char * completePath)
{
	DIR 						*dir = opendir(completePath);
	struct dirent				*entitiy;
	std::map<std::string, int>	file;
	std::string					error;

	if (dir == NULL) 
	{
		error = "the directory \"";
		error += completePath;
 		error += "\" doesnt exist";

		return (html_error(error));
	}
	entitiy = readdir(dir);
	while (entitiy != NULL)
	{
		if (entitiy->d_type == DT_DIR)
			file[entitiy->d_name] = _DIR;
		else
			file[entitiy->d_name] = _FILE;
		entitiy = readdir(dir);
	}
	closedir(dir);
	return (creat_html(file, relativePath));
}

std::string	html_error(std::string error)
{
	std::string	html;

html = "\n\
<html>\n\
 	<head>\n\
 		<title>ERROR</title>\n\
 	</head>\n\
 	<body>\n\
 		<p>";
	html += error;
	html += "</p>\n\
	</body>\n\
<html>\n";
	return (html);
}

std::string	creat_html(std::map<std::string, int> map, std::string relativePath)
{
	std::string html;
	
	if (relativePath[relativePath.size() - 1] != '/')
		relativePath += "/";
html = "\n\
<!DOCTYPE html>\n\
<html>\n\
 	<head>\n\
 		<title>ERROR</title>\n\
 	</head>\n\
 	<body>\n\
		<style>\n\
			table, td {\n\
    			border: 1px solid #333;\n\
			}\n\
			thead, tfoot {\n\
    			background-color: #333;\n\
    			color: #fff;\n\
			}\n\
			.blue {\n\
				color:blue;\n\
				text-decoration:underline;\n\
			}\n\
			.black {\n\
				color:black;\n\
				text-decoraton:none;\n\
			}\n\
			tr{\n\
				text-align:center;\n\
			}\n\
			img{\n\
				height:35px;\n\
				width:35px;\n\
			}\n\
		</style>\n\
		<table>\n\
    		<thead>\n\
        		<tr>\n\
            		<th colspan=\"2\">Index</th>\n\
        		</tr>\n\
    		</thead>\n\
    		<tbody>\n";
	for (std::map<std::string, int>::iterator it = map.begin(); it != map.end(); it ++)
	{
		html += "        		<tr>\n";
		html += "				<td><a class=\"";
		if ((*it).second == _DIR)
			html += "blue";
		else
			html += "black";
		html += "\" href=\"";
		html += relativePath + (*it).first;
		html += "\">";
		html += (*it).first;
		html += "</a></p></td>\n";
		html += "				<td><img src=\"";
		// ce que j'aimerais mettre pour que tout marche
	/*if ((*it).second == _DIR)
			html += "./html/image/dir.png";
		else
			html += "./html/image/file.png";*/	
		if ((*it).second == _DIR)
			html += "_IMAGE_/dir.png";
		else
			html += "_IMAGE_/file.png";
		html += "\" ></td>";
		html += "      		</tr>\n";
	}
	html += "\
    		</tbody>\n\
		</table>\n\
	</body>\n\
<html>\n";
	return (html);
}
