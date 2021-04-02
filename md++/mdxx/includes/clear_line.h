#ifndef MDXX_CLEAR_LINE_H
#define MDXX_CLEAR_LINE_H

#ifdef WIN32
#define MDXX_CLEAR_LINE "\r\x1b[K"
#else
#define MDXX_CLEAR_LINE "\r\033[K"
#endif


#endif
