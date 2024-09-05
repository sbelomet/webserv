# print("HTTP/1.1 200 OK")
# print("Content-type: text/plain")
# print("Content-length: 12")
# print()
# print("Hello world!")
#send(fd, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!", 74, 0);

print("HTTP/1.1 200 OK")
print("Content-type: text/html")
print("Content-length: 180")
print()
print("<!DOCTYPE html>")
print("<html>")

print("<head>")
print('<meta charset="UTF-8">')
print('<meta name="viewport" content="width=device-width, initial-scale=1.0">')
print("<title>Hello</title>")
print("</head>")

print("<body>")
print("<h2>Hello</h2>")
print("</body>")
print("</html>")