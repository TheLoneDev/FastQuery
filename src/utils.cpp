#include "utils.h"

const std::string FastQuery::ip_to_string(DWORD ipaddress)
{
	std::string ipStr;
	for (int i = 0; i < 4; i++)
	{
		UCHAR octate = ipaddress >> 8 * i & 0xff;
		ipStr.append(std::to_string(octate));
		if (i < 3)
			ipStr.append(".");
	}
	return ipStr;
}
