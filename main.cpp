#include "main.hpp"

int main(void)
{
  readAddressFile();

  return 0;
}


string readAddressFile()
{
  ifstream file(ADDR_PATH.c_str());
  string mail;

  if(file)
    {
      string line;
      while(getline(file, line))
        {
	  mail = "";
	  if(line.find("#") != 0 && line != "")
	    {
	      if(line.find("|") != string::npos) //A mail is attached to this page
		{
		  mail = line.substr(line.find("|") + 1);
		  line = line.substr(0, line.find("|"));
		}

	      cout << "Checking: " << line << "... ";
	      string online = check(line);
	      string last = readPage(line);
	      
	      if(last == "")
		writePage(line, online);
	      
	      if(error == false)
		{
		  if(last == online)
		    cout << "same" << endl;
		  else
		    {
		      cout << "DIFF: " << last.compare(online) << endl;
		      
		      if(mail != "")
			sendMail(mail, line);
		     
		      writePage(line, online);
		    }
		}
	      else
		cout << "ERROR" << endl;
	    }
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

  contents = "";
  error = false;

  curl = curl_easy_init();
  if(curl)
    {
      curl_easy_setopt(curl, CURLOPT_URL, page.c_str());
      curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,handle_data);
      curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
      curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
      curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 15);
      res = curl_easy_perform(curl);

      if(res != 0)
	{
	  error = true;
	  contents = "";
	}

      /* always cleanup */ 
      curl_easy_cleanup(curl);
    }
  else
    error = true;
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


void sendMail(string mail, string line)
{
  string command = "/usr/bin/mail -s '"+line+" has changed !' "+mail;
  FILE *mailer = popen(command.c_str(), "w");
  fprintf(mailer, "Hello %s,\n The site you are watching (%s) has just changed.\n", mail.c_str(), line.c_str());
  pclose(mailer);

  cout << "Email sent to: " << mail << endl;
}
