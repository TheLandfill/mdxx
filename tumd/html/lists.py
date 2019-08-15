#!/usr/bin/python3
from html.context import Context
from html.paragraph import paragraph_dict
from html.manager import pop, push

list_first_element = True

def open_ol(html, args, tumd):
    global list_first_element
    html.add('<ol>')
    push([html])
    list_first_element = True

def close_ol(html):
    pop([html])
    html.add('</li>')
    pop([html])
    html.add('</ol>')

def open_ul(html, args, tumd):
    global list_first_element
    html.add('<ul>')
    push([html])
    list_first_element = True

def close_ul(html):
    pop([html])
    html.add('</li>')
    pop([html])
    html.add('</ul>')

def add_li(html, line_data):
    global list_first_element
    line = line_data[0]
    blank_lines = line_data[1] > 1
    if list_first_element:
        html.add('<li>' + line)
        push([html])
        list_first_element = False
        return
    if blank_lines:
        pop([html])
        html.add('</li>')
        html.add('<li>' + line)
        push([html])
    else:
        html.add(line) 

list_dict = {
    'ol':   Context(open_ol, add_li, close_ol, paragraph_dict['paragraph'].variables),
    'ul':   Context(open_ul, add_li, close_ul, paragraph_dict['paragraph'].variables)
}
