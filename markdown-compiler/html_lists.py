#!/usr/bin/python3
from html_context import Context
from html_paragraph import html_paragraph_dict

html_list_first_element = True

def open_ol(html, args):
    global html_list_first_element
    html.add('<ol>')
    html.push()
    html_list_first_element = True

def close_ol(html):
    html.pop()
    html.add('</li>')
    html.pop()
    html.add('</ol>')

def open_ul(html, args):
    global html_list_first_element
    html.add('<ul>')
    html.push()
    html_list_first_element = True

def close_ul(html):
    html.pop()
    html.add('</li>')
    html.pop()
    html.add('</ul>')

def add_li(html, line_data):
    global html_list_first_element
    line = line_data[0]
    blank_lines = line_data[1] > 1
    if html_list_first_element:
        html.add('<li>' + line)
        html.push()
        html_list_first_element = False
        return
    if blank_lines:
        html.pop()
        html.add('</li>')
        html.add('<li>' + line)
        html.push()
    else:
        html.add(line) 

html_list_dict = {
    'ol':   Context(open_ol, add_li, close_ol, html_paragraph_dict['paragraph'].variables),
    'ul':   Context(open_ul, add_li, close_ul, html_paragraph_dict['paragraph'].variables)
}
