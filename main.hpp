#ifndef MAIN_HPP
#define MAIN_HPP
#include <iostream>
#include <curl/curl.h>
#include <fstream>

using namespace std; 

string ADDR_PATH = "./address.lst";
string PAGES_DIR = "./pages";

string contents; 


string check(string page);
string readAddressFile();
size_t handle_data(void *ptr, size_t size, size_t nmemb, void *stream) ;
string readPage(string page);
void writePage(string page, string content);
string protectString(string str);

#endif
