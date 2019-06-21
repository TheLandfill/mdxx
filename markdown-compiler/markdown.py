#!/usr/bin/python3
import sys
from collections import namedtuple

Metadata = namedtuple('Metadata', 'title author date')

class HTML_Manager:
    metadata_format = '\nPlease use the following format at the top of your article:\n\nTitle:\tHow to Write a Tutorial\nAuthor:\tJoseph Mellor\nDate:\tJune 15, 2019\nImport:\tterminal\taside\nScripts:\tdraw_spiral\tinvert_colors'

    necessary_imports = [ "general", "article" ]
    css_edges = ['<link rel="stylesheet" href="', '.css">']
    css_path = ''
    necessary_scripts = ['sidenav']

    def __init__(self, file_writer, file_reader):
        self.tab_level = ""
        self.out = file_writer
        self.in = file_reader
        self.imports = necessary_imports
        self.metadata = None
        self.scripts = necessary_scripts
        self.headings = None

    def next_line():
        return self.in.readline()
 
    def add(line):
        self.out.write(self.tab_level + line + "\n")

    def push():
        self.tab_level += "\t"

    def pop():
        self.tab_level = tab_level[:-1]
 
    def get_metadata():
        line = next_line()
        if re.match(r"^Title:\s*\S.*$", line) is None:
            print('Missing a legible title declaration.' + metadata_format)
            raise SystemExit
        self.metadata.title = re.sub('Title:\s*', '', line)
        
        line = next_line()
        if re.match(r"^Author:\s*\S.*$", line) is None:
            print('Missing a legible author declaration.' + metadata_format)
            raise SystemExit
        self.metadata.author = re.sub('Author:\s*', '', line)

        line = next_line()
        if (re.match(r"^Date:\s*\S.*$", line) is None:
            print('Missing a legible date declaration.' + metadata_format)
            raise SystemExit
        self.metadata.date = re.sub('Date:\s*', '', line)
   
    def get_imports():
        line = next_line()
        if re.match(r"^Import:.*") is None:
            print('Missing a legible import statement.' + metadata_format)
        line = re.sub('Import:', '', line)
        imports = line.split()
        self.imports.extend(imports)
    
    def get_scripts():
        line = next_line()
        if re.match(r"^Scripts:.*") is None:
            print('Missing a legible script statement.' + metadata_format)
        line = re.sub('Scripts:', '', line)
        imports = line.split()
        self.imports.extend(imports)

    def write_head():
        get_metadata()
        get_imports()
        get_scripts()
        write_generic_stuff()            # <!doctype html><html lang="en">...
        write_mandatory_stuff()          # Stuff specific to our articles
        convert_imports_to_links()
        write_title_and_close_head()
    
    def write_generic_stuff():
        add('<!doctype html>')
        add('<html lang="en">')
        push()
        add('<head>')
        push()
        add('<meta charset="utf-8">')
        add('<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">')
    
    def convert_imports_to_links():
        for css in imports:
            add(css_edges[0] + css_path + css + css_edges[1])
    
    def write_title_and_close_head():
        add('<title>' + self.metadata.title + '</title>')
        pop()
        add('</head>')
    
    def write_body_header():
        add('<body id="body-handle" onload="sidenav_height_adj()">')
        push()

    def write_article():
        add('<div class="article">')
        push()
        heading_to_section(self.metadata.title)
        add('<h1 class="display-4"><section id="' + self.headings[-1][0] + '">' + self.headings[-1][1] + '</section></h1>')
        add('<h4>' + self.metadata.author + '&middot;' + self.metadata.date)
        in_paragraph = False
        current_pre = []
        context = []
        line = ''
        while (True):
            if len(current_pre) > 0:
                line = next_line()
                if re.sub('!', '', line) == current_pre:
                    current_pre = ''
                else:
                    expanded_line = expand_line(line, current_pre)
                    self.out.write(expanded_line)
                continue
            line_data = find_next_content_line()
            line = line_data[0]
            count = line_data[1] > 1
            if line == '':
                break
            if line in pre_subs:
                current_pre.append(line)
                add(pre_subs[line] + next_line())

    def find_next_content_line():
        line = '    '
        count = 0
        while re.match('^\s*$', line) and line != '':
            count++
            line = next_line()
        return (line, count)

    def heading_to_section(heading):
        section = re.sub('\s+', '-', heading)
        section = re.sub('[^\w\-]', '', section)
        self.headings.append(section, heading)

    
    def write_sidenav():
        add('<div id="sidenav">')
        push()
        for heading in self.headings:
            add('<a href="#' + heading[0] + '">' + heading[1] + '</a>')
        pop()
    
    def write_footer():
    
    def write_scripts():
        for script in self.scripts:
            add('<script src="' + script + '"></script>')
    
    def write_end_of_file():
        add('</html>')


def convert_to_html(in, out):
    with open(in, 'r') as file_reader, open(out, 'w') as file_writer:
        manager = HTML_Manager(file_writer, file_reader)
        manager.write_head()
        manager.write_body_header()
        manager.write_article()
        manager.write_sidenav()
        manager.write_footer()
        manager.write_scripts()
        manager.write_end_of_file()

def main(args):
    """Usage:\ttu-md original.tum [filename.html]
    
    By default, the output will be a file of the same name as the markdown file,
    so "tu-md original.tum" will output to "original.html"."""
    if len(args) <= 1:
        print(main.__doc__)
        return
    
    infile = args[1]
    outfile = infile

    if len(args) >= 2:
        outfile = args[2]

    convert_to_html(infile, outfile)


if __name__ == "__main__":
    main(sys.argv)    
