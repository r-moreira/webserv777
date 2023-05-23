#!/usr/bin/env python3

import time
print("Content-Type: text/html\n\n")  # html markup follows

timeStr = time.strftime("%c") # obtains current time

htmlFormat = """
<html>
  <Title>The Time Now</Title>
<body>
  <h1>Hello from Python CGI!</h1>
  <h3>Current date and time is: {timeStr}</h3>
</body>
</html> """

print(htmlFormat.format(**locals()))  # see {timeStr} embedded above