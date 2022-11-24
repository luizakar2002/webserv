#include "./Configs.hpp"

static prop_map def_props()
{
	prop_map props;
	std::vector<string> props_list;

	props_list.push_back("80");
	props["listen"] = props_list;
	props_list.clear();

	props_list.push_back("index.html");
	props_list.push_back("index.htm");
	props["index"] = props_list;
	props_list.clear();

	return props;
}

static error_page def_errors()
{
	error_page errors;

	errors[404] = "./www/errors/404.html";
	errors[500] = "./www/errors/500.html";
	return errors;
}

Config::Config() 
{
	this->properties = def_props();
	this->error_pages = def_errors();
}

Config::Config(std::string arg, string locations)
{
	Config();
	int found = arg.find("server {");
	int k = (arg[8] == '\n') ? 9 : 8;
	if (found != arg.npos)
		arg.erase(arg.begin(), arg.begin() + k);
	if (arg[arg.size() - 1] == '}')
		arg.erase(arg.end() - 1);
	if (arg[arg.size() - 1] != '\n')
		arg.append("\n");
	this->string_to_trim = arg;
	this->parse_locations(locations, arg);
}

size_t find_trash(string str)
{
	const char *cstr = str.c_str();
	size_t pos = 0;
	while (cstr[pos] && cstr[pos] != '{' && cstr[pos] != ' ' && cstr[pos] != '\n')
		pos++;
	return pos;    
}

void Config::parse_locations(string locations,string arg) {
	stringstream locs(locations);
	vector<string> loc_list;
	string temp;
	string path;
	string line;
	size_t f;
	int count = 0, top = 0;
	while (getline(locs, line))
	{
		if (!line.compare(0, 9, "location "))
		{
			top = count;
			path = line.substr(line.find(" ") + 1);
			count++;
			if (top < count)
			{
				top = count;
				if (count > 1)
				{
					temp = loc_list[loc_list.size() - 1];
					loc_list.push_back(temp.append(path.substr(0, path.size() - 1)));
				} 
				else
					loc_list.push_back(path.substr(0, path.size() - 1));
			}
		}
		f = line.find('}');
		if (f != line.npos)
		{
			line.erase(f);
			--count;
		}
	}
	this->fill_locs_arr1(loc_list, locations);
	this->useLocations();
}

vector<string> create_prop_list(string line)
{
	vector<string> args;
	int occs = 0;

	occs = find_char_occ(line, ' ') + 1;
	for (int i = 0; i < occs; ++i)
	{
		string t = (occs - i == 1) ? line : line.substr(0, line.find(' '));
		args.push_back(t);
		if (line.find(' ') != line.npos)
			line.erase(0, line.find(' ') + 1);
		else 
			line.clear();
	}
	return args;
}

void Config::fill_locs_arr1(vector<string> loc_list, string locs)
{
	size_t pos = 0, close_pos = 0, new_pos = 0, temp_pos = 0;
	int loc_counter = 0, level = 1, index = 0;
	struct location_str temp_struct;
	pos = locs.find("location ", pos) + 9;
	++loc_counter;
	while (pos < locs.size())
	{
		close_pos = locs.find("}", pos);
		while (1)
		{
			temp_pos = locs.find("location ", pos);
			++loc_counter;
			if (close_pos < locs.size())
			{
				if (temp_pos > close_pos || temp_pos > locs.size())
				{
					pos = close_pos;
					break;
				}
				else 
				{
					++level;
					new_pos = locs.find("location ", pos);
					pos = new_pos + 9;
				}
			}
		}
		temp_struct.level = level;
		temp_struct.loc = locs.substr(new_pos, close_pos - new_pos + 1);
		loc_strs.push_back(temp_struct);
		locs.erase(new_pos, close_pos - new_pos + 1);
		if (temp_pos == locs.npos)
		{
			temp_struct.level = 1;
			temp_struct.loc = locs;
			loc_strs.push_back(temp_struct);
			break;
		}
		temp_pos = 0;
		new_pos = 0;
		pos = locs.find("location ", 0) + 9;
		close_pos = 0;
		loc_counter -= level;
		level = 1;
	}
}

static  std::vector<location_str>::iterator find_next_highest(vector<location_str> *v)
{
	std::vector<location_str>::iterator iter = v->begin();
	
	while(iter != v->end())
	{
		if (iter->level == 1)
			return iter;
		++iter;
	}
	return iter;
}
static void print_ep(error_page l)
{

	error_page::iterator corey = l.begin();
	if (corey == l.end())
		std::cout << "datarka ara:)\n";
	while (corey != l.end())
	{
		std::cout << "Code: " << corey->first;
		std::cout << " Path: " << corey->second;
		std::cout << "\n";
		++corey;
	}
}

Location Config::jdi_menya(string s)
{
	int i = 0;

	while(i < this->locations.size() && this->locations[i].get_path() != s)
		++i;
	return this->locations[i];
	
}

void Config::splitndestroy(string loc, string temp1)
{
	string temp;
	string key, error_path;
	vector<string> args;
	prop_map temp_map;
	error_page temp_error;
	Location loc_temp;
	int last_slash = 0, code;
	int occs;

	loc.erase(0, loc.find('{') + 1);
	if (loc[loc.size() - 1] == '}')
		loc.erase(loc.size() - 1);
	last_slash = temp1.find_last_of('/');
	if (this->locations.empty() || !last_slash)
	{
		loc_temp.set_errors(this->get_errors());
		loc_temp.set_path(temp1);
	}
	else if (last_slash > 0)
	{
		loc_temp = jdi_menya(temp1.substr(0, last_slash));
		loc_temp.set_path(temp1);	
	}
	temp_map = loc_temp.get_props();
	temp_error = loc_temp.get_errors();
	while(1) 
	{
		temp = loc.substr(0, loc.find("\n"));
		key = temp.substr(0, temp.find(' '));
		temp.erase(0, temp.find(' ') + 1);
		if (key == "error_page")
		{
			removeSpaces(temp);
			code = stoi(temp.substr(0, temp.find(' ')));
			temp.erase(0, temp.find(' ') + 1);
			temp_error[code] = temp;
			args.clear();
			loc.erase(0, loc.find("\n") + 1);
			if (loc.empty())
				break;
			continue;

		}
		occs = find_char_occ(temp, ' ') + 1;
		for (int i = 0; i < occs; ++i)
		{
			string t = (occs - i == 1) ? temp : temp.substr(0, temp.find(' '));
			args.push_back(t);
			if (temp.find(' ') != temp.npos)
				temp.erase(0, temp.find(' ') + 1);
			else 
				temp.clear();
		}
		temp_map[key] = args;
		args.clear();
		loc.erase(0, loc.find("\n") + 1);
		if (loc.empty())
			break;
	}
	temp_map.erase(temp_map.begin());
	loc_temp.set_props(temp_map);
	loc_temp.set_errors(temp_error);
	this->locations.push_back(loc_temp);
}


void Config::fillaret(std::vector<location_str>::iterator iter, Location &temp_loc)
{
	std::vector<location_str>::iterator temp_iter = iter;
	int elems = 0, level_diff = 0, i = 0;
	static int index = 0;
	string temp;
	stack<string> temp_paths;

	temp = iter->loc.substr(iter->loc.find(" ") + 1, iter->loc.find('{') - iter->loc.find(" ") - 2);
	if (temp[temp.size() - 1] == '/')
		temp.erase(temp.size() - 1);
	temp_paths.push(temp);
	elems = std::distance(this->loc_strs.begin(), iter) + 1;
	while (i++ < elems)
	{
		if (iter != this->loc_strs.begin())
			iter--;
		if (iter->level > temp_iter->level)
		{
			splitndestroy(temp_iter->loc, temp);
			if (iter + 1 == loc_strs.end())
				break;
			temp = temp_paths.top() + iter->loc.substr(iter->loc.find(" ") + 1, iter->loc.find('{') - iter->loc.find(" ") - 2);
			if (temp[temp.size() - 1] == '/')
				temp.erase(temp.size() - 1);
			temp_paths.push(temp);
		}
		else
		{
			level_diff = temp_iter->level - iter->level + 1;
			while (level_diff--)
				temp_paths.pop();
			splitndestroy(temp_iter->loc, temp);
			if (iter + 1 == loc_strs.end())
				break;
			temp = temp_paths.top() + iter->loc.substr(iter->loc.find(" ") + 1, iter->loc.find('{') - iter->loc.find(" ") - 2);
			if (temp[temp.size() - 1] == '/')
				temp.erase(temp.size() - 1);
			temp_paths.push(temp);
		}
		--temp_iter;
		level_diff = 0;
	}
}

void Config::useLocations() 
{
	Location temp;
	std::vector<location_str>::iterator iter;
	
	while (!this->loc_strs.empty())
	{
		iter = find_next_highest(&this->loc_strs);
		fillaret(iter, temp);
		this->loc_strs.erase(this->loc_strs.begin(), iter + 1);
	}
}

const prop_map &Config::get_map()const 
{
	return this->properties;
}

error_page Config::get_errors()const
{
	return this->error_pages;
}

std::vector<Location> Config::get_locations()const
{
	return this->locations;
}

void Config::add_error(string key, string arg)
{
	int code;

	removeSpaces(arg);
	code = stoi(arg.substr(0, arg.find(' ')));
	arg.erase(0, arg.find(' ') + 1);
	this->error_pages[code] = arg;
}

void Config::string_to_map()
{
	string temp;
	string key;
	vector<string> args;
	int occs;
	int i = 0;

	while(1) 
	{
		++i;
		temp = this->string_to_trim.substr(0, this->string_to_trim.find("\n"));
		key = temp.substr(0, temp.find(' '));
		temp.erase(0, temp.find(' ') + 1);
		if (key == "error_page")
			add_error(key, temp);
		occs = find_char_occ(temp, ' ') + 1;
		for (int i = 0; i < occs; ++i)
		{
			string t = (occs - i == 1) ? temp : temp.substr(0, temp.find(' '));
			args.push_back(t);
			if (temp.find(' ') != temp.npos)
				temp.erase(0, temp.find(' ') + 1);
			else 
				temp.clear();
		}
		this->properties[key] = args;
		args.clear();
		this->string_to_trim.erase(0, this->string_to_trim.find("\n") + 1);
		if (this->string_to_trim.empty())
			break;
	}
}

Config::~Config() {}
