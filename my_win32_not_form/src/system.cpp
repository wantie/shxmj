#include "system.h"
std::wstring strsyserror()
{
	TCHAR buffer[1024];
	int ret = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
							NULL,
							GetLastError(),
							0,
							buffer,
							1024,
							NULL);
	if (ret > 0)
		return buffer;
	return TEXT("");
}