#!/usr/bin/python3
import re
import importlib
from collections import namedtuple
from html.default import default_dict
import html.manager
from pprint import pprint

Metadata = namedtuple('Metadata', 'title author date')

class Non_Article_Manager:
    metadata_format = '\nPlease use the following format at the top of your article:\n\nTitle:\tHow to Write a Tutorial\nAuthor:\tJoseph Mellor\nDate:\tJune 15, 2019\nImport:\tterminal\taside\nScripts:\tdraw_spiral\tinvert_colors\nCode Style:\tdefault'

    necessary_css = [ "general", "article" ]
    css_edges = ['<link rel="stylesheet" href="', '.css">']
    css_path = '../../../css/'
    code_style_path = 'code-style/'
    necessary_scripts = ['sidenav']
    scripts_path = '../../../js/'

    def __init__(self, html_manager):
        self.html = html_manager
        self.css = Non_Article_Manager.necessary_css
        self.imports = []
        self.scripts = Non_Article_Manager.necessary_scripts
        self.metadata = None 

    def get_tags(self, tag_list):
        data = []
        for tag in tag_list:
            line = self.html.next_line_no_nl()
            re_test = r'^' + tag[0] + r':\s*\S.*'
            if re.match(re_test, line) is None and tag[1]:
                print('Missing a legible ' + tag[0].lower() + ' declaration.' + Non_Article_Manager.metadata_format)
                raise SystemExit
            data.append(re.sub(tag[0] + r':\s*', '', line))
        return data

    def get_metadata(self):
        metadata_tags = [(r'Title', True), (r'Author', True), (r'Date', True)]
        data = self.get_tags(metadata_tags)
        self.metadata = Metadata(title=data[0], author=data[1], date=data[2])
   
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
       
    def write_head(self):
        self.get_metadata()
        self.get_imports()
        self.copy_raw_html('snippets_html/generic.html')
        self.copy_raw_html('snippets_html/mandatory.html')
        self.convert_imports_to_links()
        self.import_contexts()
        self.write_title_and_close_head()
        self.copy_raw_html('snippets_html/body_header.html')
        self.write_start_of_article()

    def add(self, line):
        self.html.add(line)

    def push(self):
        self.html.push()

    def pop(self):
        self.html.pop()

    def copy_raw_html(self, infile):
        with open(infile, 'r') as file_reader:
            line = re.sub('\n','', file_reader.readline())
            while line != '':
                if line == 'tumd_pop()':
                    self.pop()
                elif line == 'tumd_push()':
                    self.push()
                else:
                    self.add(line)
                line = re.sub('\n', '', file_reader.readline())

    def convert_imports_to_links(self):
        for css in self.css:
            self.add(Non_Article_Manager.css_edges[0] + self.css_path + css + Non_Article_Manager.css_edges[1])

    def import_contexts(self):
        for plugin in self.imports:
            print('Importing ' + plugin)
            context_dict = importlib.import_module('plugins.' + plugin).context_dict
            print()
            self.add_context(context_dict)
            for key in context_dict:
                for key2 in default_dict:
                    if key2 not in context_dict[key].variables:
                        context_dict[key].variables[key2] = default_dict[key2]

    def add_context(self, local_context):
        context_dict = self.html.context_dict
        self.html.context_dict = { **local_context, **context_dict }

    def write_title_and_close_head(self):
        self.add('<title>' + self.metadata.title + '</title>')
        self.pop()
        self.add('</head>')

    def write_start_of_article(self):
        self.add('<div class="article">')
        self.push()
        self.html.heading_to_section(self.metadata.title, 0)
        self.add('<h1 class="display-4" id="' + self.html.headings[-1][0] + '">' + self.html.headings[-1][1] + '</h1>')
        self.add('<h4>' + self.metadata.author + '&middot;' + self.metadata.date + '</h4>')

    def write_end(self):
        self.copy_raw_html('snippets_html/footer.html')
        self.write_scripts()
        self.copy_raw_html('snippets_html/end_of_file.html')

    def write_scripts(self):
        for script in self.scripts:
            self.add('<script src="' + Non_Article_Manager.scripts_path + script + '.js"></script>')
