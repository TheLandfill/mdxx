#include "mdxx_ansi.h"

#ifdef WIN32
const char * MDXX_VAR_COLOR = "\x1b[38;2;128;255;200m";
const char * MDXX_VAL_COLOR = "\x1b[38;2;255;153;255m";
const char * MDXX_RESET = "\x1b[0m";
const char * MDXX_RESET_COLOR = "\x1b[39m";
const char * MDXX_RESET_FORMATTING = "\x1b[0m";
const char * MDXX_CONTEXT_COLOR = "\x1b[1m\x1b[38;2;159;135;255m";
const char * MDXX_LINE_COLOR = "\x1b[1m\x1b[38;2;254;217;255m";
const char * MDXX_ERROR_COLOR = "\x1b[38;2;255;121;0m";
const char * MDXX_ERROR_PREFIX = "\x1b[1m\x1b[4mERROR:";
const char * MDXX_WARNING_COLOR = "\x1b[38;2;255;250;205m";
const char * MDXX_WARNING_PREFIX = "\x1b[1m\x1b[4mWARNING:";
const char * MDXX_CLEAR_LINE = "\r\x1b[K";
#else
const char * MDXX_VAR_COLOR = "\033[38;2;128;255;200m";
const char * MDXX_VAL_COLOR = "\033[38;2;255;153;255m";
const char * MDXX_RESET = "\033[0m";
const char * MDXX_RESET_COLOR = "\033[39m";
const char * MDXX_RESET_FORMATTING = "\033[0m";
const char * MDXX_CONTEXT_COLOR = "\033[1m\033[38;2;159;135;255m";
const char * MDXX_LINE_COLOR = "\033[1m\033[38;2;254;217;255m";
const char * MDXX_ERROR_COLOR = "\033[38;2;255;121;0m";
const char * MDXX_ERROR_PREFIX = "\033[1m\033[4mERROR:";
const char * MDXX_WARNING_COLOR = "\033[38;2;255;250;205m";
const char * MDXX_WARNING_PREFIX = "\033[1m\033[4mWARNING:";
const char * MDXX_CLEAR_LINE = "\r\033[K";
#endif

