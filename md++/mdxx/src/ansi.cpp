#include "mdxx_ansi.h"

#ifdef WIN32
const char * MDXX_VAR_COLOR = "\x1b[38;2;128;255;200m";
const char * MDXX_VAL_COLOR = "\x1b[38;2;255;153;255m";
const char * MDXX_RESET = "\x1b[m";
const char * MDXX_CONTEXT_COLOR = "\x1b[1m\x1b[38;2;159;135;255m";
const char * MDXX_LINE_COLOR = "\x1b[1m\x1b[38;2;254;217;255m";
const char * MDXX_ERROR_PREFIX = "\x1b[1m\x1b[4m\x1b[38;2;255;121;0mERROR: = ";
const char * MDXX_WARNING_PREFIX = "\x1b[1m\x1b[4m\x1b[38;2;255;250;205mWARNING: = ";
const char * MDXX_CLEAR_LINE = "\r\x1b[K";
#else
const char * MDXX_VAR_COLOR = "\033[38;2;128;255;200m";
const char * MDXX_VAL_COLOR = "\033[38;2;255;153;255m";
const char * MDXX_RESET = "\033[m";
const char * MDXX_CONTEXT_COLOR = "\033[1m\033[38;2;159;135;255m";
const char * MDXX_LINE_COLOR = "\033[1m\033[38;2;254;217;255m";
const char * MDXX_ERROR_PREFIX = "\033[1m\033[4m\033[38;2;255;121;0mERROR: = ";
const char * MDXX_WARNING_PREFIX = "\033[1m\033[4m\033[38;2;255;250;205mWARNING: = ";
const char * MDXX_CLEAR_LINE = "\r\033[K";
#endif

