#include "main.hpp"

int main(void)
{
  readAddressFile();

  return 0;
}


string readAddressFile()
{
  ifstream file(ADDR_PATH.c_str());

  if(file)
    {
      string line;
      while(getline(file, line))
        {
	  cout << "Checking: " << line << "..." << endl;
	  string online = check(line);
	  string last = readPage(line);
	  if(last.compare(online))
	    {
	      cout << "SAME" << endl;
	    }
        }
    }
  else
      cout << "ERROR: AddressFile not found: " << ADDR_PATH << endl;

  return contents;
}

string readPage(string page)
{
  string filename = PAGES_DIR+"/"+page.substr(7);

  ifstream file(filename.c_str());
  string out = "";
  if(file)
    {
      string line;
      while(getline(file, line))
        { 
	  out += line;
        }
    }
  else
      cout << "Warning: page file not found: " << filename << endl;

  return out;
}

string check(string page)
{
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  if(curl)
    {
      curl_easy_setopt(curl, CURLOPT_URL, page.c_str());
      curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,handle_data);
      res = curl_easy_perform(curl);

      /* always cleanup */ 
      curl_easy_cleanup(curl);
    }

  return contents;
}


size_t handle_data(void *ptr, size_t size, size_t nmemb, void *stream) 
{ 
  int numbytes = size*nmemb; 
  char lastchar = *((char *) ptr + numbytes - 1); 
  *((char *) ptr + numbytes - 1) = '\0'; 
  contents.append((char *)ptr); 
  contents.append(1,lastchar); 
  *((char *) ptr + numbytes - 1) = lastchar; //Useful ?

  return size*nmemb; 
}
