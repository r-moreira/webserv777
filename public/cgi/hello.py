#!/usr/bin/env python3

import time
import os

print("Content-Type: text/html\n\n")

current_date_time = time.strftime("%c")

browser = os.environ['HTTP_USER_AGENT']

query_string = os.environ['QUERY_STRING']

name = 'World'
if query_string.startswith('name='):
    name = query_string[5:]

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

print(html_format.format(**locals()))