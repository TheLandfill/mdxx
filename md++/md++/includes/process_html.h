#ifndef MDXX_PROCESS_HTML_H
#define MDXX_PROCESS_HTML_H
#include "html_manager.h"
#include "line_data.h"

namespace mdxx {

void process_html(HTML_Manager& html, const Line_Data& line_data, bool& in_pre_section);

}

#endif
