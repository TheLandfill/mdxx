#!/usr/bin/python3
from html_context import Context
from html_paragraph import html_paragraph_dict

def open_ol(html, args):
    html.add('<ol>')
    html.push()

def close_ol(html):
    html.pop()
    html.add('</ol>')

def open_ul(html, args):
    html.add('<ul>')
    html.push()

def close_ul(html):
    html.pop()
    html.add('</ul>')

def add_li(html, line):
    html.add('<li>' + line[0] + '</li>') 

html_list_dict = {
    'ol':   Context(open_ol, add_li, close_ol, html_paragraph_dict['paragraph'].variables),
    'ul':   Context(open_ul, add_li, close_ul, html_paragraph_dict['paragraph'].variables)
}
