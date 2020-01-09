#!/usr/bin/python3
import re
import importlib
from html.default import default_dict
import html.manager
from tumd_manager import TUMD_Manager
from pprint import pprint
from template.context import template_dict
import os

class Template_Manager:
    metadata_format = '\nPlease use the following format at the top of your article:\n\nTitle:\tHow to Write a Tutorial\nAuthor:\tJoseph Mellor\nDate:\tJune 15, 2019\nImport:\tterminal\taside\nScripts:\tdraw_spiral\tinvert_colors\nCode Style:\tdefault'

    css_edges = ['<link rel="stylesheet" href="', '.css">']

    def __init__(self, html_manager, content_manager):
        self.html = html_manager
        self.css = []
        self.imports = []
        self.scripts = []
        self.metadata = None
        self.content = content_manager
        self.tumd = self.content.tumd
        self.tumd.find_next_content_line()
        template = self.tumd.line_data[0]
        self.template = TUMD_Manager(open('templates/' + template + '.tumd', 'r'))
        self.template.context_dict = template_dict
        self.template.context_dict['template'].variables['template'] = self
        self.template.context_dict['template'].variables['content'] = self.content
        self.template.context_dict['template'].variables['self'] = self.content
        self.template.context_dict['template'].variables['html'] = self.html
        self.template.context_dict['template'].variables['path'] = os.path.dirname(os.path.realpath(self.content.tumd.infile.name))
        self.tumd.context_dict['raw-html'].variables['html'] = self.html
        self.template.context = ['default', 'template']
        self.template.add_default_variables()

    def get_tags(self, tag_list):
        data = []
        for tag in tag_list:
            line = self.tumd.next_line_no_nl()
            re_test = r'^' + tag[0] + r':\s*\S.*'
            if re.match(re_test, line) is None and tag[1]:
                print('Missing a legible ' + tag[0].lower() + ' declaration.' + Template_Manager.metadata_format)
                raise SystemExit
            data.append(re.sub(tag[0] + r':\s*', '', line))
        return data

    def get_imports(self):
        import_tags = [(r'Import', False), (r'Scripts', False)]
        data = self.get_tags(import_tags)
        imports = data[0].split()
        self.imports += imports
        self.css += imports
        scripts = data[1].split()
        self.scripts += scripts

    def get_data(args):
        self = args[0]
        self.get_imports()
        self.import_contexts()
        return ''

    def process_template(self):
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
        template = args[0]
        css_path = template.template.context_dict['template'].variables['css']
        for css in template.css:
            args[0].add('<link rel="stylesheet" href="' + css_path + css + '.css">')
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
        template = args[0]
        js_path = template.template.context_dict['template'].variables['js']
        for script in template.scripts:
            template.add('<script src="' + js_path + script + '.js"></script>')
        return ''
