#include "Response.hpp"
#include "MAIN.hpp"


Response::Response(Request *request, Config &conf)
{
    // this->status_code needs to be taken from request as well
    this->version = "HTTP/1.1 ";
    this->request = request;
    this->config = conf;
    std::string uri = "";
    prop_map map = this->config.get_map();
    const std::string prop = "listen";
    std::vector<std::string>::iterator vect = map[prop].begin();
    std::string ext = "";


    uri = this->request->get_uri();
    if (request->get_status_code() != 200)
        error(request->get_status_code());
    ext_mime(uri, ext);
    fill_phrase();

    find_correct_location(uri);
    finder(uri, ext);
    
    fill_status_line();
    fill_headers();


}

void Response::ext_mime(std::string &uri, std::string &ext)
{

    hasExt(uri, ext);
    if (ext == "css")
        this->mime = "text/css";
    else if (ext == "csv")
        this->mime = "text/csv";
    else if (ext == "htm")
        this->mime = "text/html";
    else if (ext == "html")
        this->mime = "text/html";
    else if (ext == "ico")
        this->mime = "image/x-icon";
    else if (ext == "jpeg")
        this->mime = "image/jpeg";
    else if (ext == "jpg")
        this->mime = "image/jpeg";
    else if (ext == "js")
        this->mime = "application/javascript";
    else if (ext == "json")
        this->mime = "application/json";
    else if (ext == "png")
        this->mime = "image/png";
    else if (ext == "pdf")
        this->mime = "application/pdf";
    else if (ext == "svg")
        this->mime = "image/svg+xml";
    else if (ext == "txt")
        this->mime = "text/plain";
    // else if (ext == "php")
    //     this->mime = "application/x-httpd-php";
    else
        this->mime = "text/html";
}

void Response::find_correct_location(std::string &uri)
{
    std::vector<Location> loc_iter = this->config.get_locations();
    int i = 0;
    this->location = loc_iter[loc_iter.size() - 1];
    while (i < loc_iter.size())
    {
        if (loc_iter[i].get_path() == uri)
        {
            this->location = loc_iter[i];
            break;
        }
        ++i;
    }
}

void Response::error(int status)
{
    const error_page errs = this->location.get_errors();
    this->status_code = status;

    fill_phrase();
    if (errs.count(status))
    {
        open_static(errs.find(status)->second);
    }
}

void Response::finder(std::string &uri, std::string &ext)
{
    std::string temp1;
    std::vector<std::string> temp_file_iter;
    size_t match = 0;
    int i =0;
    std::vector<std::string> v = this->location.get_props().find("methods")->second;

    if (this->location.get_props().count("methods") && (std::count(v.begin(), v.end(), this->request->get_method())))
    {
        if (hasExt(uri, ext) && (ext == "html" || ext == "htm"))
        {
            open_file(uri);
            return ;
        }
        else if (hasExt(uri, ext) && is_non_static(uri))
        {
            serve_non_static(uri);
            return ;
        }
        else if (hasExt(uri, ext))
        {
            open_file(uri);
            return ;
        }
        else
        {
            if (!this->location.get_props().count("index") && check_autoindex())
            {
                create_autoindex();
            }
            else
            {
                std::vector<Location> loc_iter = this->config.get_locations();
                int i = 0;

                while (i < loc_iter.size())
                {
                    if (loc_iter[i].get_path() == uri)
                        break;
                    ++i;
                }
                if (i == loc_iter.size())
                {
                    error(404);
                    return ;
                }
                if (this->location.get_props().count("index"))
                    for (int i = 0; i < this->location.get_props().find("index")->second.size(); i++)
                    {
                        temp_file_iter = this->location.get_props().find("index")->second;
                        if ((match = this->location.get_props().find("index")->second[i].find_last_of(".")) < temp_file_iter[i].size())//stic kanes ara, es qo inchn em asel!!!!!!!!!!!!!!!
                        {
                            temp1 = temp_file_iter[i].substr(match + 1, temp_file_iter[i].size() - match);
                            if (temp1 == "html" || temp1 == "htm")
                            {
                                if (isExist(this->location.get_props().find("root")->second[0] + "/" + temp_file_iter[i]))
                                {
                                    open_static(temp_file_iter[i]);
                                    return ;
                                }
                            }
                            else if (is_non_static(uri))
                            {
                                serve_non_static(uri);
                                return ;
                            }

                        }
                    }
                if (check_autoindex())
                {
                    create_autoindex();
                    return ;
                }
                else
                {
                    this->error(404);
                    return ;
                }
            }
        }
    }
    else 
    {
        error(403);
        return ;
    }
}

void Response::create_autoindex()
{
	std::string body = "";
	body += "<!DOCTYPE html>\n";
	body += "<html>\n";
	body += "<head>\n";
	body += "</head>\n";
	body += "<body>";
	body += "autoindex page\n\n";
	body += "<br><br>";
	if (makeFileList(body))
    {
        body += "</body>\n";
	    body += "</html>\n";
        this->content += body;
        return ;
    }	
    body += "</body>\n";
    body += "</html>\n";
    this->content += body;
}

int Response::makeFileList(std::string &body)
{
    DIR *dir = NULL;
	struct dirent *directory = NULL;


	if ((dir = opendir((this->location.get_props().find("root")->second[0] + this->request->get_uri()).c_str())) == NULL)
    {
		this->error(500);
        return 0;
    }
	while ((directory = readdir(dir)))
	{
		std::string file_name(directory->d_name);
		if (file_name != ".")
		{
			body += "<a href=http://" + this->request->get_headers().find("Host")->second + this->request->get_uri() + "/" + file_name + " >" + file_name + "</a><br>";
		}
	}
	closedir(dir);
    return 1;
}

int Response::check_autoindex()
{
    prop_map::const_iterator map = this->location.get_props().find("index");
    int flag = 0;
    std::vector<const std::string>::iterator indexs;
    std::string root;

    if (this->location.get_props().count("root"))
        root = this->location.get_props().find("root")->second[0];
    if (map != this->location.get_props().end())
        indexs = map->second.begin();
    else
        flag = 1;
    if (this->location.get_props().count("autoindex") && this->location.get_props().find("autoindex")->second[0] == "on")
    {
    //     if (this->location.get_props().count("index"))
    //         return 0; //1 er, poxecinq mb sxal
        while ( flag != 1 && this->location.get_props().count("index") && indexs != this->location.get_props().find("index")->second.end())
        {
            if (isExist(root + "/" + *indexs))
                return 0;
            indexs++;
        }
        return 1;
    }
    return 0;
}

Response::~Response()
{
}

void Response::open_file(std::string uri)
{
    std::string path = this->location.get_props().find("root")->second[0] + "/" + uri;

    std::ifstream file(path.c_str());
    if ( (file.rdstate() & std::ifstream::failbit ) != 0
    || (file.rdstate() & std::ifstream::badbit ) != 0 )
    {
        error(404);
    }

    
    std::stringstream body;
    body << file.rdbuf();
    this->content = body.str();

    file.close();
}

char    *Response::get_c_response()
{
    return this->c_response;
}

void    Response::fill_phrase()
{
    if (this->status_code == 200)
        this->phrase = "OK";
    else if (this->status_code == 403)
        this->phrase = "Forbidden resource";
    else if (this->status_code == 404)
        this->phrase = "Not found";
    else if (this->status_code == 500)
        this->phrase = "Internal server error";
    else
        this->phrase = "KO(";
}

void    Response::fill_status_line()
{
    this->status_line += this->version + std::to_string(this->status_code) + " " + this->phrase + "\r\n";
    this->raw_response += this->status_line;
}

void    Response::fill_headers()
{
    this->raw_response += "Connection: keep-alive\nContent-Type: "+this->mime+"\nContent-Length: ";
    this->raw_response += std::to_string(this->content.size()) + "\n\n";
    this->raw_response += this->content;

    this->c_response = (char *)this->raw_response.c_str();
}

void    Response::serve_non_static(std::string file_uri)
{

    std::vector<std::string> cgi_pass;

    std::string cgi_ext = "";
    std::string cgi_path = "";
    std::string temp = this->request->get_uri();

    if (config.get_map().count("cgi_pass") && (cgi_pass = config.get_map().find("cgi_pass")->second).size() == 2)
    {
        cgi_ext = cgi_pass.at(0);
        cgi_path = cgi_pass.at(1);
        CGI cgi = CGI(cgi_ext, cgi_path, file_uri, &config, request);
        this->content = cgi.get_reponse();
    }
    else
        this->error(500);
}

void   Response::open_static(std::string file)
{
    std::string line;
    std::string path = this->location.get_props().find("root")->second[0] + "/" + file;
    std::ifstream static_file(path, std::ios::binary);

    if (static_file.is_open())
    {
        while (static_file)
        {
            std::getline(static_file, line);
            this->content += line;
        }
        static_file.close();
    }
    else
    {
        this->error(404);
    }
}

int Response::is_non_static(std::string uri)
{
    std::string ext = uri.substr(uri.find_first_of(".", 0) + 1, uri.size() - uri.find_first_of(".", 0));
    std::vector<Location> locs = this->config.get_locations();
    std::vector<Location>::iterator locs_iter = locs.begin();

    if (this->location.get_props().count("cgi_pass") && (*this->location.get_props().find("cgi_pass")->second.begin() == ext))
        return 1;
    
    while (locs_iter != locs.end())
    {
        if (locs_iter->get_path() == uri)
        {
            if (locs_iter->get_props().count("cgi_pass"))
                return (1);
        }
        locs_iter++;
    }
    return 0;
}


int Response::hasExt(std::string &uri, std::string &ext)
{
    size_t match = 0;
    std::string temp_file;
    std::vector<const std::string>::iterator temp_file_iter;

    if (((match = uri.find_first_of("?", 0)) < uri.size()))
        uri.erase(match, uri.size() - match);
    if (((match = uri.find_last_of(".")) < uri.npos) && match > uri.find_last_of("/"))
    {
        ext = uri.substr(match + 1, uri.size() - match); 
        return (1);
    }
    return 0;
}