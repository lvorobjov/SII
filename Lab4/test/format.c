// format.c
// Using FormatMessage

#include <windows.h>

#define BUFFER_SIZE 64

void GetFormattedMessage(LPTSTR pBuffer, LPTSTR pMessage, ...)
{
    va_list args = NULL;
    va_start(args, pMessage);

    FormatMessage(FORMAT_MESSAGE_FROM_STRING,
                  pMessage, 
                  0,  // игнорируется
                  0,  // игнорируется
                  (LPWSTR)&pBuffer, 
                  BUFFER_SIZE, 
                  &args);

    va_end(args);
}