#!/usr/bin/python3
import re
from html.context import Context
from html.manager import pop, push

in_pre_section = False
html_variables = {
    'html' : '',
    'pop' : pop,
    'push' : push,
    'tupo' : '{{pop (html)}}',
    'tupu' : '{{push (html)}}'
}

def open_html(html, args, tumd):
    html_variables['html'] = html
    return

def process_html(html, line_data):
    global in_pre_section
    html_variables['html'] = html
    if in_pre_section:
        for i in range(1, line_data[1]):
            html.add_pre('')
        html.add_pre(line_data[0])
    else:
        html.add(line_data[0])
    if re.search('<pre>', line_data[0]):
        in_pre_section = True
    if re.search('</pre>', line_data[0]):
        in_pre_section = False

def close_html(html):
    return

raw_html_dict = {
    'raw-html': Context(open_html, process_html, close_html, html_variables)
}
