#include <stdio.h>
#include "window.h"
#include "utils.h"

#if !defined(_WIN32) && !defined(_WIN64)
#ifdef __LINUX
#error Linux not supported yet
#else
#error Only Windows is supported at the moment
#endif
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windns.h>
#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#pragma comment(lib, "dnsapi.lib")
#endif
#endif

std::string cb_GetRecord(const char *, FastQuery::RecordType);

int main()
{
	FastQuery::Window window;
	if (!window.Initialize(cb_GetRecord))
	{
		SHOWMESSAGE("Error Initializing Window");
		return -1;
	}
	window.Show();
	window.Run();

	return 0;
}

std::string cb_GetRecord(const char *address, FastQuery::RecordType recordType)
{
	PDNS_RECORDA records;
	WORD type = 0;
	switch (recordType)
	{
	case FastQuery::RecordType::A:
		type = DNS_TYPE_A;
		break;
	case FastQuery::RecordType::AAAA:
		type = DNS_TYPE_AAAA;
		break;
	case FastQuery::RecordType::CAA:
		type = DNS_TYPE_CERT;
		break;
	case FastQuery::RecordType::CNAME:
		type = DNS_TYPE_CNAME;
		break;
	case FastQuery::RecordType::MX:
		type = DNS_TYPE_MX;
		break;
	case FastQuery::RecordType::NS:
		type = DNS_TYPE_NS;
		break;
	case FastQuery::RecordType::PTR:
		type = DNS_TYPE_PTR;
		break;
	case FastQuery::RecordType::SOA:
		type = DNS_TYPE_SOA;
		break;
	case FastQuery::RecordType::SRV:
		type = DNS_TYPE_SRV;
		break;
	case FastQuery::RecordType::TXT:
		type = DNS_TYPE_TEXT;
		break;
	}
	DNS_STATUS res = DnsQuery_UTF8(address, type, DNS_QUERY_BYPASS_CACHE, 0, (PDNS_RECORD *)&records, 0);
	if (res || !records)
	{
		return "Failed to query dns record";
	}

	if (recordType == FastQuery::RecordType::MX && records->wType == DNS_TYPE_MX)
	{
		if (records->Data.MX.pNameExchange)
		{
			return std::to_string(records->Data.MX.wPreference).append(":").append(records->Data.MX.pNameExchange);
		}
	}

	return FastQuery::ip_to_string(records->Data.A.IpAddress);
}