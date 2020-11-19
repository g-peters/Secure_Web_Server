


static char body[] = {

"<html><body><head>"
"<link rel = \"icon\" href = \"data:,\">" // stops favicon requests

"<h1>Server Responded</h1>"
"<p>More content to be added</p>"

"</head>"
"</body>"
"</html>\n"
};

static char header[] = {
	"HTTP/1.0 200 OK \r\n"
	"Content-Type: text/html; charset=utf-8\r\n"
	"Connection: close \r\n"
	"\r\n"
};
