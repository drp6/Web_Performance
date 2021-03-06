#include "Net_time.hpp"

// The max time (seconds) for an operation to be performed. 0 = 300 seconds (default)
const long TIME_OUT = 3;

// Constructors
Net_time::Net_time(IObtain_urls* obtain_urls, IMetric* metric, ILog* log) :
_obtain_urls (obtain_urls), _metric (metric), _log (log)
{update_urls();}

// Used for suppression of HTML output with CURL commands
static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
   return size * nmemb;
}

// Performs the query for the given metric on the specified URLs
// The results are stored in the _results vector until cleared.
// The errors are stored in the _errors vector until cleared
void Net_time::execute() {

    for (const auto& url : _urlList) {
        CURL* curl;
    	CURLcode result_code;

    	curl = curl_easy_init();
    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // Suppresses nasty output
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // Sets URL

        // Added for testing/ display purposes
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIME_OUT);

    	result_code = curl_easy_perform(curl); // Checks URL

    	if(result_code == CURLE_OK) { // URL is good, proceed to time metric
            double time_returned;
    		result_code = _metric->execute(curl, time_returned); // Calls for set metric

            if (result_code == CURLE_OK) { // Send results to _results
                _results.push_back(std::make_pair(url, time_returned));
            }
            else {  // Send URL and error message to _errors
                std::string error_msg =  curl_easy_strerror(result_code);
                _errors.push_back(std::make_pair(url, error_msg));
            }
    	}
        else { // Send URL and error message to _errors
            std::string error_msg =  curl_easy_strerror(result_code);
            _errors.push_back(std::make_pair(url, error_msg));
        }

    	curl_easy_cleanup(curl);
	}
}

void Net_time::update_urls() {_urlList = _obtain_urls->obtain_urls();}
void Net_time::results_log() {_log->results_log(_results);}
void Net_time::errors_log() {_log->errors_log(_errors);}
void Net_time::clear_results() {_results.clear();}

void Net_time::set_obtain_urls(IObtain_urls* obtain_urls) { _obtain_urls = obtain_urls;}
void Net_time::set_log(ILog* log) {_log = log;}
void Net_time::set_metric(IMetric* metric) {_metric = metric;}
