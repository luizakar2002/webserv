#include "Location.hpp"
#include "./Configs.hpp"

Location::Location(): depth(1) {}

Location::Location(string &l)
{

}

Location::Location(const Location &l)
{
    this->path = l.path;
    this->depth = l.depth;
    this->properties = l.properties;
    this->error_pages = l.error_pages;

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
		// break;
	}
}

// void print_vector(vector<string> v)
// {
//     vector<string>::iterator uf = v.begin();
//     while(uf != v.end())
//     {
//         std::cout << *uf << " ";
//         ++uf;
//     }
//     std::cout << "\n";
// }

// void print_map(const prop_map &props)
// {
//     prop_map::const_iterator begin = props.cbegin();
//     while(begin != props.end())
//     {
//         std::cout << "KEY: " << begin->first << " ==== ";
//         print_vector(begin->second);
//         ++begin;
//     }
// }

Location &Location::operator=(const Location &l)
{
    this->path = l.path;
    this->depth = l.depth;
    this->properties = l.properties;
    // print_map(l.properties);
    this->error_pages = l.error_pages;
    // std::cout << "this isi \n";
    // print_ep(l.error_pages);
    return *this;
}

string Location::get_path() const
{ return this->path; }

void Location::set_path(string path)
{ this->path = path; }

prop_map Location::get_props() const
{ return this->properties; }

void Location::set_props(prop_map props)
{ this->properties = props; }

int Location::get_depth()const
{ return this->depth;}

void Location::set_depth(int d)
{ this->depth = d;}

void Location::set_errors(error_page ep)
{
    this->error_pages = ep;
}

error_page Location::get_errors()const
{
    // error_page::const_iterator corey = this->error_pages.begin();
    // std::cout << "GET_ERRORS-UM\n\n";
	// if (corey ==  this->error_pages.end())
	// 	std::cout << "datarka ara:)\n";
	// while (corey !=  this->error_pages.end())
	// {
	// 	std::cout << "Code: " << corey->first;
	// 	std::cout << "Path: " << corey->second;
	// 	std::cout << "\n";
	// 	++corey;
	// 	// break;
	// }
    // std::cout << "==========================\n";
    // print_ep(this->error_pages);
    return this->error_pages;
}

Location::~Location() {}
