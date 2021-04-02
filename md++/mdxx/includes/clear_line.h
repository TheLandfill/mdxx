#ifndef MDXX_CLEAR_LINE_H
#define MDXX_CLEAR_LINE_H

#ifdef WIN32
#define MDXX_ESC "\x1b"
#else
#define MDXX_ESC "\033"
#endif

#define MDXX_CLEAR_LINE "\r" MDXX_ESC "[K"

#endif
