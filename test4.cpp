#include <iostream>
#include <string>
#include <curl/curl.h>

size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
	size_t realsize = size * nmemb;
	output->append((char*)contents, realsize);
	return realsize;
}

std::string getPublicIP() {
	CURL *curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		if(res != CURLE_OK) {
			std::cerr << "Erreur lors de la requête HTTP : " << curl_easy_strerror(res) << std::endl;
			return "";
		}
	}

	return readBuffer;
}

int main() {
	curl_global_init(CURL_GLOBAL_ALL);
	std::string publicIP = getPublicIP();
	curl_global_cleanup();

	if (!publicIP.empty())
	{
		std::cout << "Adresse IP publique : " << publicIP << std::endl;
	} else
	{
		std::cerr << "Impossible de récupérer l'adresse IP publique" << std::endl;
	}

	return 0;
}
