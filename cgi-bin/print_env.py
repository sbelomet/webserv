import os

print("<!DOCTYPE html>")
print("<html>")

print("\t<head>")
print('\t<meta charset="utf-8">')
print("\t<title>Env</title>")
print("\t</head>")

print("\t<body>")
for key, value in os.environ.items():
	print(f"\t\t<p>{key}: {value}</p>")
print("\t</body>")
print("</html>")