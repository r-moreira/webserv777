#!/usr/bin/env python3

import time
import os
import sys
import urllib.parse


def get_body():
    _body = ""
    if os.environ.get('REQUEST_METHOD') == 'POST':
        content_length = int(os.environ.get('CONTENT_LENGTH'))
        _body = sys.stdin.read(content_length)
    return _body


current_date_time = time.strftime("%c")
method = os.environ.get('REQUEST_METHOD')
query_params = urllib.parse.parse_qs(os.environ.get('QUERY_STRING'))
body_params = urllib.parse.parse_qs(get_body())
browser = os.environ.get('HTTP_USER_AGENT')
message = ""
password = ""
name = "World"

if method == 'GET':
    name = query_params['name'][0] if 'name' in query_params else name
    message = query_params['message'][0] if 'message' in query_params else message

if method == 'POST':
    name = body_params['name'][0] if 'name' in body_params else name
    message = body_params['message'][0] if 'message' in body_params else message
    password = body_params['password'][0] if 'password' in body_params else password

if browser is None or len(browser) == 0:
    browser = 'Unknown'

html_format = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Python CGI</title>
</head>
<html>
  <Title>Python CGI!</Title>
<body>
  <h1>Hello, {name}!</h1>
  <h3>Current date and time is: {current_date_time}</h3>
  <h3>Your browser is: {browser}</h3>
  <h3>{message}</h3>
</body>
</html> 
"""

html_format_error = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Python CGI</title>
</head>
<body>
    <h1>CGI Form Test</h1>
    <hr>
    <form ACTION="http://localhost:8082/cgi/" METHOD="POST">
    Enter your user: <input TYPE="text" NAME="user" SIZE=60><br>
    Enter your password: <input TYPE="password" NAME="password" SIZE=60><br>
    <p>
        <input TYPE="submit" VALUE="Submit the form">
        <input TYPE="reset"  VALUE="Clear all fields">
    </form>
    <h3>Wrong password, try again!</h3>
    <hr>
</body>
</html>
"""

print("Content-Type: text/html\n\n")

if method == 'GET':
    print(html_format.format(**locals()))
else:
    if password == "42":
        print(html_format.format(**locals()))
    else:
        print(html_format_error.format(**locals()))
