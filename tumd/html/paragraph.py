#!/usr/bin/python3
from html.context import Context

context_dict = {
}

def paragraph(html, line_data):
    line = line_data[0]
    blank_lines = line_data[1] > 1
    if blank_lines:
        html.check_and_close_paragraph()
        html.add('<p>' + line)
        html.push()
        html.need_to_close_paragraph = True
    else:
        html.add(html.expand_line(line))

paragraph_dict = {
    'paragraph':    Context(None, paragraph, None, context_dict)
}

