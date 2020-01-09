#!/usr/bin/env python3
from tumd_manager import TUMD_Manager
from util.author import author_name
from html.context import Context
from html.default import default_dict
from html.manager import pop, push

root = 'https://tuacm.com/blog/'

def get_article_tags(path):
    tag_list = None
    with open(path + '/tags.txt', 'r') as file_reader:
        tag_list = file_reader.read.splitlines()
    return tag_list

def get_tagged_articles(path):
    global root
    articles = None
    with open(path + '/tagged.txt', 'r') as file_reader:
        articles = file_reader.read().splitlines()
        root = articles[0]
        articles = articles[1:]
    return articles

def print_tagged_articles(args):
    html = args[0]
    article_box_variables['html'] = html
    path = args[1]
    articles = get_tagged_articles(path)
    for article in articles:
        article = article.split('|')
        article_box_variables['title'] = article[0]
        article_box_variables['author'] = article[1]
        article_box_variables['tagline'] = article[2]
        article_box_variables['directory'] = root + article[3]
        article_box_variables['author-name'] = author_name(article_box_variables['author'])
        with open("templates/article-box.tumd", "r") as reader:
            tumd = TUMD_Manager(reader)
            tumd.context_dict = article_box_dict
            tumd.context = ['default', 'article-box' ]
            while (True):
                tumd.find_next_content_line()
                if tumd.line_data[0] == '':
                    break
                tumd.handle_context(html)
    return ''

article_box_variables = {
    'title' : None,
    'author' : None,
    'author-link' : '{{author-name (author)}}',
    'author-name' : author_name,
    'tagline' : None,
    'directory' : None,
    'pop' : pop,
    'push' : push,
    'tupu' : '{{push (html)}}',
    'tupo' : '{{pop (html)}}'
}

article_box_variables = { **default_dict, **article_box_variables }

def process_article_box(html, line_data):
    html.add(line_data[0])

article_box_dict = {
    'article-box': Context(None, process_article_box, None, article_box_variables)
}
