#!/usr/bin/python3
from html_context import Context

def html_paragraph(html, line_data):
    line = line_data[0]
    blank_lines = line_data[1] > 1
    if blank_lines:
        html.check_and_close_paragraph()
        html.add('<p>' + html.expand_line(line))
        html.push()
        html.need_to_close_paragraph = True
    else:
        html.add(html.expand_line(line))

html_paragraph_dict = {
    'paragraph':    Context(None, html_paragraph, None)
}

