#!/usr/bin/python3
from html.context import Context

def open_comment(html, args):
    print('\033[48;5;156mEntering commented section. These next lines are commented out.\033[m')
    html.context = html.context[:-1]
    while html.line_data[0] != r'\{{\comment}}' and html.line_data[0] != '':
        html.find_next_content_line()
        print(html.line_data[0])
    if html.line_data[0] == '':
        print('ERROR: missing \{{\comment}} tag.')
        raise SystemExit
    print()

def process_comment(html, line_data):
    return

def close_comment(html):
    return

comment_dict = {
    'comment': Context(open_comment, process_comment, close_comment, {})
}
