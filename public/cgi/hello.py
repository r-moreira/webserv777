#!/usr/bin/env python3

import time
import os
import sys


def get_name():
    _name = 'World'
    if os.environ.get('REQUEST_METHOD') == 'POST':
        content_length = int(os.environ.get('CONTENT_LENGTH'))
        body = sys.stdin.read(content_length)
        _name = body[5:]

    if os.environ.get('REQUEST_METHOD') == 'GET':
        query_string = os.environ.get('QUERY_STRING')
        if query_string is not None and query_string.startswith('name='):
            _name = query_string[5:]
    return _name


name = get_name()
current_date_time = time.strftime("%c")
browser = os.environ.get('HTTP_USER_AGENT')
if browser is None:
    browser = 'Unknown'

html_format = """
<html>
  <Title>Python CGI!</Title>
<body>
  <h1>Hello, {name}!</h1>
  <h3>Current date and time is: {current_date_time}</h3>
  <h3>Your browser is: {browser}</h3>
</body>
</html> 
"""

print("Content-Type: text/html\n\n")
print(html_format.format(**locals()))
