#!/usr/bin/python3
from html_context import Context

def open_ol(html):
    html.add('<ol>')
    html.push()

def close_ol(html):
    html.pop()
    html.add('</ol>')
    html.context = html.context[:-1]

def open_ul(html):
    html.add('<ul>')
    html.push()

def close_ul(html):
    html.pop()
    html.add('</ul>')
    html.context = html.context[:-1]

def add_li(html, line):
    html.add('<li>' + line[0] + '</li>') 

html_list_dict = {
    'ol':   Context(open_ol, add_li, close_ol),
    'ul':   Context(open_ul, add_li, close_ul)
}
