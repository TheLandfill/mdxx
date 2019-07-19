#!/usr/bin/python3
from html.context import Context

def open_comment(html, args, tumd):
    print('\033[48;5;156mEntering commented section. These next lines are commented out.\033[m')
    tumd.context = tumd.context[:-1]
    tumd.find_next_content_line()
    while tumd.line_data[0] != r'\{{\comment}}' and tumd.line_data[0] != '':
        tumd.print_line()
        tumd.find_next_content_line()
    if tumd.line_data[0] == '':
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
