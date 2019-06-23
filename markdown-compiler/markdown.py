#!/usr/bin/python3
import sys
import re
from collections import namedtuple

Metadata = namedtuple('Metadata', 'title author date')

pre_subs = {}

class HTML_Manager:
    metadata_format = '\nPlease use the following format at the top of your article:\n\nTitle:\tHow to Write a Tutorial\nAuthor:\tJoseph Mellor\nDate:\tJune 15, 2019\nImport:\tterminal\taside\nScripts:\tdraw_spiral\tinvert_colors'

    necessary_imports = [ "general", "article" ]
    css_edges = ['<link rel="stylesheet" href="', '.css">']
    css_path = ''
    necessary_scripts = ['sidenav']

    def __init__(self, file_writer, file_reader):
        self.tab_level = ""
        self.out = file_writer
        self.infile = file_reader
        self.imports = HTML_Manager.necessary_imports
        self.metadata = None
        self.scripts = HTML_Manager.necessary_scripts
        self.headings = []

    def next_line(self):
        return self.infile.readline()

    def next_line_no_nl(self):
        return re.sub('\n', '', self.next_line())
 
    def add(self, line):
        self.out.write(self.tab_level + line + "\n")

    def push(self):
        self.tab_level += "\t"

    def pop(self):
        self.tab_level = self.tab_level[:-1]
 
    def get_metadata(self):
        line = self.next_line_no_nl()
        if re.match(r"^Title:\s*\S.*$", line) is None:
            print('Missing a legible title declaration.' + HTML_Manager.metadata_format)
            raise SystemExit
        temp_title = re.sub('Title:\s*', '', line)
        print('|' + temp_title + '|')
        
        line = self.next_line_no_nl()
        if re.match(r"^Author:\s*\S.*$", line) is None:
            print('Missing a legible author declaration.' + HTML_Manager.metadata_format)
            raise SystemExit
        temp_author = re.sub('Author:\s*', '', line)
        print('|' + temp_author + '|')

        line = self.next_line_no_nl()
        if re.match(r"^Date:\s*\S.*$", line) is None:
            print('Missing a legible date declaration.' + HTML_Manager.metadata_format)
            raise SystemExit
        temp_date = re.sub('Date:\s*', '', line)
        print('|' + temp_date + '|')
        self.metadata = Metadata(title=temp_title, author=temp_author, date=temp_date)
   
    def get_imports(self):
        line = self.next_line_no_nl()
        if re.match(r"^Import:.*", line) is None:
            print('Missing a legible import statement.' + HTML_Manager.metadata_format)
            raise SystemExit
        line = re.sub('Import:', '', line)
        imports = line.split()
        self.imports.extend(imports)
    
    def get_scripts(self):
        line = self.next_line_no_nl()
        if re.match(r"^Scripts:.*", line) is None:
            print('Missing a legible script statement.' + HTML_Manager.metadata_format)
            raise SystemExit
        line = re.sub('Scripts:', '', line)
        imports = line.split()
        self.imports.extend(imports)

    def write_head(self):
        self.get_metadata()
        self.get_imports()
        self.get_scripts()
        self.write_generic_stuff()            # <!doctype html><html lang="en">...
        self.write_mandatory_stuff()          # Stuff specific to our articles
        self.convert_imports_to_links()
        self.write_title_and_close_head()

    def write_body(self):
        self.write_body_header()
        self.write_article()
        self.write_sidenav()
        self.write_footer()
        self.close_body()
        self.write_scripts()
        self.write_end_of_file()

    def write_mandatory_stuff(self):
        return
    
    def write_generic_stuff(self):
        self.add('<!doctype html>')
        self.add('<html lang="en">')
        self.push()
        self.add('<head>')
        self.push()
        self.add('<meta charset="utf-8">')
        self.add('<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">')
    
    def convert_imports_to_links(self):
        for css in self.imports:
            self.add(HTML_Manager.css_edges[0] + self.css_path + css + HTML_Manager.css_edges[1])
    
    def write_title_and_close_head(self):
        self.add('<title>' + self.metadata.title + '</title>')
        self.pop()
        self.add('</head>')
    
    def write_body_header(self):
        self.add('<body id="body-handle" onload="sidenav_height_adj()">')
        self.push()

    def write_article(self):
        self.add('<div class="article">')
        self.push()
        self.heading_to_section(self.metadata.title)
        self.add('<h1 class="display-4"><section id="' + self.headings[-1][0] + '">' + self.headings[-1][1] + '</section></h1>')
        self.add('<h4>' + self.metadata.author + '&middot;' + self.metadata.date)
        current_pre = []
        context = ['none']
        line = ''
        need_to_close_paragraph = False
        while (True):
            if len(current_pre) > 0:
                line = self.next_line()
                if re.sub('!', '', line) == current_pre:
                    current_pre = current_pre[:-1]
                else:
                    expanded_line = expand_line(line, context)
                    self.out.write(expanded_line)
                continue
            line_data = self.find_next_content_line()
            line = line_data[0]
            blank_lines = line_data[1] > 1
            if line == '':
                break
            if line in pre_subs:
                current_pre.append(line)
                context.append(line)
                self.add(pre_subs[line] + self.next_line_no_nl())
            if len(context) == 1:
                if blank_lines:
                    if need_to_close_paragraph:
                        self.pop()
                        self.add('</p>')
                        need_to_close_paragraph = False
                    self.add('<p>' + line)
                    self.push()
                    need_to_close_paragraph = True
                else:
                    self.add(line)
            elif need_to_close_paragraph:
                self.pop()
                self.add('</p>')
                need_to_close_paragraph = False
        if need_to_close_paragraph:
            self.pop()
            self.add('</p>')
            need_to_close_paragraph = False
        self.pop()
        self.add('</div>')


    def find_next_content_line(self):
        line = '    '
        count = 0
        while re.match('^\s*$', line) and line != '':
            count += 1
            line = self.next_line()
        line = re.sub('\n', '', line)
        return (line, count)

    def heading_to_section(self, heading):
        section = re.sub('\s+', '-', heading)
        section = re.sub('[^\w\-]', '', section)
        self.headings.append((section.lower(), heading))
    
    def write_sidenav(self):
        self.add('<div id="sidenav">')
        self.push()
        for heading in self.headings:
            self.add('<a href="#' + heading[0] + '">' + heading[1] + '</a>')
        self.pop()
        self.add('</div>')
    
    def write_footer(self):
        return

    def close_body(self):
        self.pop()
        self.add('</body>')
    
    def write_scripts(self):
        for script in self.scripts:
            self.add('<script src="' + script + '.js"></script>')
    
    def write_end_of_file(self):
        self.pop()
        self.add('</html>')


def convert_to_html(infile, out):
    with open(infile, 'r') as file_reader, open(out, 'w') as file_writer:
        manager = HTML_Manager(file_writer, file_reader)
        manager.write_head()
        manager.write_body()

def main(args):
    """Usage:\ttu-md original.tumd [filename.html]
    
    By default, the output will be a file of the same name as the markdown file,
    so "tu-md original.tumd" will output to "original.html"."""
    if len(args) <= 1:
        print(main.__doc__)
        return
    
    infile = args[1]
    if infile[-5:] != '.tumd':
        print(main.__doc__)
        return
    outfile = re.sub('tumd', 'html', infile)

    if len(args) >= 3:
        outfile = args[2]

    convert_to_html(infile, outfile)


if __name__ == "__main__":
    main(sys.argv)    
