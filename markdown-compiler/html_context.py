#!/usr/bin/python3

class Context:
    def __init__(self, open_context, process_context, close_context):
        self.open_context = open_context
        self.process_context = process_context
        self.close_context = close_context
