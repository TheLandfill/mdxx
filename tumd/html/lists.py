#!/usr/bin/python3
from html.context import Context
from html.paragraph import paragraph_dict

list_first_element = True

def open_ol(html, args):
    global list_first_element
    html.add('<ol>')
    html.push()
    list_first_element = True

def close_ol(html):
    html.pop()
    html.add('</li>')
    html.pop()
    html.add('</ol>')

def open_ul(html, args):
    global list_first_element
    html.add('<ul>')
    html.push()
    list_first_element = True

def close_ul(html):
    html.pop()
    html.add('</li>')
    html.pop()
    html.add('</ul>')

def add_li(html, line_data):
    global list_first_element
    line = line_data[0]
    blank_lines = line_data[1] > 1
    if list_first_element:
        html.add('<li>' + line)
        html.push()
        list_first_element = False
        return
    if blank_lines:
        html.pop()
        html.add('</li>')
        html.add('<li>' + line)
        html.push()
    else:
        html.add(line) 

list_dict = {
    'ol':   Context(open_ol, add_li, close_ol, paragraph_dict['paragraph'].variables),
    'ul':   Context(open_ul, add_li, close_ul, paragraph_dict['paragraph'].variables)
}
