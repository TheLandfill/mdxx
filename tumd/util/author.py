#/usr/bin/python3
from urllib.request import urlopen
from html.manager import pop, push
from tumd_manager import TUMD_Manager
from html.context import Context
from html.default import default_dict

path = 'https://tuacm.com/authors/'

def get_author_image(args):
    name = args[0]
    return author_name(name) + '/large.jpg'

def write_author_description(args):
    meta = args[0]
    author_variables['author'] = meta.template.cur_context_vars()['author']
    html = meta.html
    author_variables['html'] = html
    with open("templates/author-description.tumd", "r") as reader:
        tumd = TUMD_Manager(reader)
        tumd.context_dict = author_dict
        tumd.context = ['default', 'author-description']
        while (True):
            tumd.find_next_content_line()
            if tumd.line_data[0] == '':
                break
            tumd.handle_context(html)
    return ''

def get_author_description(args):
    author = author_variables['author']
    author_url = author_name(author) + '/description.txt'
    print(author_url)
    data = urlopen(author_url)
    description = data.readline().decode("utf-8")
    for line in data:
        description += '\t' + line.decode("utf-8")
    return description

author_variables = {
    'author-image' : get_author_image,
    'author' : 'MISSING',
    'author-description-text' : get_author_description,
    'html' : None,
    'pop' : pop,
    'push' : push,
    'tupu' : '{{push (html)}}',
    'tupo' : '{{pop (html)}}'
}

author_variables = { **default_dict, **author_variables }

def process_author(html, line_data):
    html.add(line_data[0])

author_dict = {
    'author-description': Context(None, process_author, None, author_variables)
}

def author_name(name):
    return path + '-'.join(name.split()).lower()
