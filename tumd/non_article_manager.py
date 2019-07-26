#!/usr/bin/python3
import re
import importlib
from html.default import default_dict
import html.manager
from tumd_manager import TUMD_Manager
from pprint import pprint
from non_article import non_article_dict

class Non_Article_Manager:
    metadata_format = '\nPlease use the following format at the top of your article:\n\nTitle:\tHow to Write a Tutorial\nAuthor:\tJoseph Mellor\nDate:\tJune 15, 2019\nImport:\tterminal\taside\nScripts:\tdraw_spiral\tinvert_colors\nCode Style:\tdefault'

    necessary_css = [ "general", "article" ]
    css_edges = ['<link rel="stylesheet" href="', '.css">']
    css_path = '../../../css/'
    code_style_path = 'code-style/'
    necessary_scripts = ['sidenav']
    scripts_path = '../../../js/'

    def __init__(self, html_manager, article_manager):
        self.html = html_manager
        self.css = Non_Article_Manager.necessary_css
        self.imports = []
        self.scripts = Non_Article_Manager.necessary_scripts
        self.metadata = None
        self.article = article_manager
        self.tumd = self.article.tumd
        self.template = TUMD_Manager(open('templates/article.tumd', 'r'))
        self.template.context_dict = non_article_dict
        self.template.context_dict['non-article'].variables['meta'] = self
        self.template.context_dict['non-article'].variables['article'] = self.article
        self.template.context = ['default', 'non-article']
        self.template.add_default_variables()

    def get_tags(self, tag_list):
        data = []
        for tag in tag_list:
            line = self.tumd.next_line_no_nl()
            re_test = r'^' + tag[0] + r':\s*\S.*'
            if re.match(re_test, line) is None and tag[1]:
                print('Missing a legible ' + tag[0].lower() + ' declaration.' + Non_Article_Manager.metadata_format)
                raise SystemExit
            data.append(re.sub(tag[0] + r':\s*', '', line))
        return data

    def get_metadata(self):
        metadata_tags = [(r'Title', True), (r'Author', True), (r'Descrip', True)]
        data = self.get_tags(metadata_tags)
        self.template.cur_context_vars()['title'] = data[0]
        self.template.cur_context_vars()['author'] = data[1]
        self.template.cur_context_vars()['description'] = data[2]
   
    def get_imports(self):
        import_tags = [(r'Import', False), (r'Scripts', False), (r'Code Style', False)]
        data = self.get_tags(import_tags)
        imports = data[0].split()
        self.imports += imports
        self.css += imports
        scripts = data[1].split()
        self.scripts += scripts
        code_style = data[2]
        code_style = re.sub('\s*$', '', code_style)
        if code_style != '':
            self.html.code_style = re.sub(r'\+.*', '', code_style)
        self.css.append(Non_Article_Manager.code_style_path + code_style)

    def get_data(self):
        self.get_metadata()
        self.get_imports()
        self.import_contexts()
       
    def process_template(self):
        self.get_data()
        while (True):
            self.template.find_next_content_line()
            if self.template.line_data[0] == '':
                break
            self.template.handle_context(self.html)

    def add(self, line):
        self.html.add(line)

    def push(self):
        self.html.push()

    def pop(self):
        self.html.pop()

    def convert_imports_to_links(args):
        for css in args[0].css:
            args[0].add(Non_Article_Manager.css_edges[0] + args[0].css_path + css + Non_Article_Manager.css_edges[1])
        return ''

    def import_contexts(self):
        for plugin in self.imports:
            print('Importing ' + plugin)
            context_dict = importlib.import_module('plugins.' + plugin).context_dict
            print()
            self.add_context(context_dict)
        self.tumd.add_default_variables()

    def add_context(self, local_context):
        context_dict = self.tumd.context_dict
        self.tumd.context_dict = { **local_context, **context_dict }

    def write_scripts(args):
        for script in args[0].scripts:
            args[0].add('<script src="' + Non_Article_Manager.scripts_path + script + '.js"></script>')
        return ''
