#ifndef KOTERET_UTILS
#define KOTERET_UTILS
#include <string>
typedef unsigned long DWORD;
typedef unsigned char UCHAR;


namespace FastQuery
{
	enum RecordType
	{
		A, AAAA, CNAME, MX, NS, PTR, SOA, SRV, TXT, CAA
	};
	const static char* RTStrings[] = { "A", "AAAA", "CNAME", "MX", "NS", "PTR", "SOA", "SRV", "TXT", "CAA" };
	const std::string ip_to_string(DWORD ipaddress);
}

#endif