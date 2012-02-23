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
	  cout << "Checking: " << line << "... ";
	  string online = check(line);
	  string last = readPage(line);

	  if(last == "")
	      writePage(line, online);

	  //	  cout << online << endl;
	  if(online.find("ERROR") != 0)
	    {
	      if(last == online)
		cout << "same" << endl;
	      else
		{
		  cout << "DIFF: " << last.compare(online) << endl;
		  writePage(line, online);
		}
	    }
	  else
	    cout << online << endl;
        }
    }
  else
      cout << "ERROR: AddressFile not found: " << ADDR_PATH << endl;

  return contents;
}

void writePage(string page, string content)
{
  page = protectString(page.substr(7));
  string filename = PAGES_DIR+"/"+page;
  ofstream file(filename.c_str());
  
  if(file)    
      file << content;
  else
      cout << "ERROR: cannot open file: " << filename << endl;

  file.close();
}

string readPage(string page)
{
  page = protectString(page.substr(7));
  string filename = PAGES_DIR+"/"+page;

  ifstream file(filename.c_str());
  string out = "";
  if(file)
    {
      string line;
      while(getline(file, line))
        { 
	  out += line+"\n";
        }
    }
  //  else
  //    cout << "Warning: page file not found: " << filename << endl;
  return out;
}

string check(string page)
{
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  if(curl)
    {
      contents = "";
      curl_easy_setopt(curl, CURLOPT_URL, page.c_str());
      curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,handle_data);
      curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
      curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
      curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 15);
      res = curl_easy_perform(curl);

      if(res != 0)
	  contents = "ERROR: "+res;

      /* always cleanup */ 
      curl_easy_cleanup(curl);
    }
  //  cout << contents << endl;
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


string protectString(string str)
{
  for (int i = 0; i < str.length(); ++i) {
    if (str[i] == '.' || str[i] == '/' || str[i] == '?' || str[i] == ':')
      str[i] = '-';
  }
  return str;
}
