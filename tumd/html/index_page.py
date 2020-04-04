#!/usr/bin/env python3
import os
import re
from html.manager import push, pop
from template.manager import Template_Manager
from templates.list import template_list, skipable
from util.author import author_name

def get_immediate_subdirectories(a_dir):
    return [name for name in os.listdir(a_dir)
            if os.path.isdir(os.path.join(a_dir, name))]

def get_metadata(args):
    template = args[0]
    metadata_tags = [(r'Topic', True)]
    tumd = template.template
    data = template.get_tags(metadata_tags)
    tumd.cur_context_vars()['topic'] = data[0]
    return ''

def get_description(args):
    content = args[0]
    content.tumd.context = [ 'default', 'description' ]
    description = ''
    content.tumd.find_next_content_line()
    if content.tumd.line_data[0] == '':
        return
    description += content.tumd.line_data[0].rstrip() + ' '
    while (True):
        content.tumd.find_next_content_line()
        if content.tumd.line_data[0] == '' or content.tumd.line_data[1] > 1:
            break
        description += content.tumd.line_data[0].rstrip() + ' '
    description = re.sub(r"<.?>", "", description)
    return description.rstrip()

def get_tags(infile, template):
    data = {}
    taglist = template_list[template]
    for tag in taglist:
        line = infile.readline().rstrip()
        re_test = r'^' + tag + r':\s*\S.*'
        if re.match(re_test, line) is None:
            print('Missing a legible ' + tag.lower())
            raise SystemExit
        data[tag] = re.sub(tag + r':\s*', '', line)
    if template == 'index-page':
        description = ''
        while (True):
            line = infile.readline()
            if line == '':
                break
            if line in skipable:
                continue
            description += line.rstrip() + ' '
        data['Description'] = re.sub('\s+', ' ', description.rstrip())
    return data

def print_subfolders(args):
    html = args[0]
    content = args[1]
    maindir = os.path.dirname(os.path.realpath(content.tumd.infile.name))
    dirs = get_immediate_subdirectories(maindir)
    dirs.sort()
    articles = []
    for directory in dirs:
        try:
            with open(maindir + '/' + directory + '/index.tumd', 'r') as file_reader:
                line = file_reader.readline().rstrip()
                if line == 'article':
                    content_dict = get_tags(file_reader, line)
                    content_dict['Directory'] = directory
                    articles.append(content_dict)
        except:
            pass
    if len(articles) > 0:
        html.add('<h1 class="section-head">Articles</h1>')
    for article in articles:
        html.add('<div class="article-box">')
        push([html])
        html.add('<h2 class="article-box-text"><a href="' + article['Directory'] + '">' + article['Title'] + '</a></h2><br>')
        html.add('<h4 class="article-box-text">by <a href="' + author_name(article['Author']) + '">' + article['Author'] + '</a></h4><br>')
        html.add('<span style="font-size: 1.25em">' + article['Tagline'] + '</span>')
        html.add('<a href="' + article['Directory'] + '" class="article-box-link"></a>')
        pop([html])
        html.add('</div>')
    return ''
