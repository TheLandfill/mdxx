#!/usr/bin/python3
import re
import sys
from html.manager import HTML_Manager
from non_article_manager import Non_Article_Manager
from tumd_manager import TUMD_Manager
from html.article import Article_Manager

def convert_to_html(infile, out):
    with open(infile, 'r') as file_reader, open(out, 'w') as file_writer:
        tumd = TUMD_Manager(file_reader)
        writer = HTML_Manager(file_writer)
        article = Article_Manager(writer, tumd)
        meta = Non_Article_Manager(writer, article)
        meta.process_template()

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
