#ifndef IMetric_h
#define IMetric_h

#include <curl/curl.h>

//////////////////////////////////////////////////////////////////////////////////////////
// Metric you are tracking interface

class IMetric {
public:
	virtual CURLcode execute(CURL* curl, double& returned_time) = 0;
	virtual ~IMetric(){}
};

//////////////////////////////////////////////////////////////////////////////////////////

// Concrete classes

//////////////////////////////////////////////////////////////////////////////////////////
//  The TTFB class is for measuring the time from start until first byte is received.
class TTFB : public IMetric {
	CURLcode execute(CURL* curl, double& returned_time);
};

//////////////////////////////////////////////////////////////////////////////////////////
// The Response_time class is for measuring the time between when the SSL/SSH handshake
// is completed and the time the first byte is received.

class Response_time : public IMetric {
	CURLcode execute(CURL* curl, double& returned_time);
};

//////////////////////////////////////////////////////////////////////////////////////////

#endif
