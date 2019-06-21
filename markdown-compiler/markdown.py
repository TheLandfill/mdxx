#!/usr/bin/python3
import sys
from pprint import pprint

def write_header_information(file_writer):
    write_generic_stuff(file_writer)            # <!doctype html><html lang="en">...
    write_mandatory_stuff(file_writer)          # Stuff specific to our articles

def convert_to_html(in, out):
    with open(in, 'r') as file_reader, open(out, 'w') as file_writer:
        imports = get_imports(file_reader)
        meta_data = get_metadata(file_reader)
        


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
