#!/usr/bin/python3
import re
from collections import namedtuple
import html_manager

Metadata = namedtuple('Metadata', 'title author date')

class Non_Article_Manager:
    metadata_format = '\nPlease use the following format at the top of your article:\n\nTitle:\tHow to Write a Tutorial\nAuthor:\tJoseph Mellor\nDate:\tJune 15, 2019\nImport:\tterminal\taside\nScripts:\tdraw_spiral\tinvert_colors'

    necessary_imports = [ "general", "article" ]
    css_edges = ['<link rel="stylesheet" href="', '.css">']
    css_path = '../css/'
    necessary_scripts = ['sidenav']
    scripts_path = '../js/'

    def __init__(self, html_manager):
        self.html = html_manager
        self.imports = Non_Article_Manager.necessary_imports
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
        import_tags = [(r'Import', False), (r'Scripts', False)]
        data = self.get_tags(import_tags)
        imports = data[0].split()
        self.imports += imports
        scripts = data[1].split()
        self.scripts += scripts
       
    def write_head(self):
        self.get_metadata()
        self.get_imports()
        self.write_generic_stuff()
        self.write_mandatory_stuff()
        self.convert_imports_to_links()
        self.write_title_and_close_head()
        self.write_body_header()
        self.write_start_of_article()

    def add(self, line):
        self.html.add(line)

    def push(self):
        self.html.push()

    def pop(self):
        self.html.pop()

    def write_generic_stuff(self):
        self.add('<!doctype html>')
        self.add('<html lang="en">')
        self.push()
        self.add('<head>')
        self.push()
        self.add('<meta charset="utf-8">')
        self.add('<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">')

    def write_mandatory_stuff(self):
        self.add('<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">')

    def convert_imports_to_links(self):
        for css in self.imports:
            self.add(Non_Article_Manager.css_edges[0] + self.css_path + css + Non_Article_Manager.css_edges[1])

    def write_title_and_close_head(self):
        self.add('<title>' + self.metadata.title + '</title>')
        self.pop()
        self.add('</head>')

    def write_body_header(self):
        self.add('<body id="body-handle" onload="sidenav_height_adj()">')
        self.push()
        self.add('<div id="webpage-header" class="jumbotron jumbotron-fluid">')
        self.push()
        self.add('<div class="container">')
        self.push()
        self.add('<a href="../">&larr; Back To Resources</a>')
        self.pop()
        self.add('</div>')
        self.pop()
        self.add('</div>')
        self.add('<div>')
        self.push()

    def write_start_of_article(self):
        self.add('<div class="article">')
        self.push()
        self.html.heading_to_section(self.metadata.title)
        self.add('<h1 class="display-4"><section id="' + self.html.headings[-1][0] + '">' + self.html.headings[-1][1] + '</section></h1>')
        self.add('<h4>' + self.metadata.author + '&middot;' + self.metadata.date)

    def write_end(self):
        self.write_footer()
        self.close_body()
        self.write_scripts()
        self.write_end_of_file()

    def write_footer(self):
        self.pop()
        self.add('</div>')
        self.add('<div id="footer" style="height: 20vh; background: black; width: 100%; z-index: 100;"></div>')

    def close_body(self):
        self.pop()
        self.add('</body>')

    def write_scripts(self):
        for script in self.scripts:
            self.add('<script src="' + Non_Article_Manager.scripts_path + script + '.js"></script>')

    def write_end_of_file(self):
        self.pop()
        self.add('</html>')

