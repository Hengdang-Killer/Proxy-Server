/*
 * proxy_parse.h -- a HTTP Request Parsing Library.
 *
 * Written by: Matvey Arye
 * For: COS 518 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include <ctype.h>

#ifndef PROXY_PARSE
#define PROXY_PARSE

#define DEBUG 1

// This defines the ParsedRequest structure, which holds the parsed elements of an HTTP request
struct ParsedRequest {
     char *method; // HTTP method (e.g., GET, POST).
     char *protocol; // The protocol used (e.g., http).
     char *host; // The host address (e.g., www.example.com).
     char *port; // The port number (e.g., 80).
     char *path; // The requested path (e.g., /index.html).
     char *version; // The HTTP version (e.g., HTTP/1.1).
     char *buf; // The buffer that holds the request data.
     size_t buflen; // The length of the buffer.
     struct ParsedHeader *headers; // A linked list of headers, where each header is a key-value pair.
     size_t headersused; // The number of headers currently stored.
     size_t headerslen; // The maximum number of headers that can be stored.
};

// This defines the ParsedHeader structure, which holds a single header from an HTTP request
struct ParsedHeader {
     char * key; // The header's key (e.g., Content-Length).
     size_t keylen; // The length of the key.
     char * value; // The header's value (e.g., 80 for Content-Length).
     size_t valuelen; // The length of the value.
};

// Declares a function ParsedRequest_create that returns a pointer to a ParsedRequest structure. 
// This function will create and initialize an empty ParsedRequest object.
struct ParsedRequest* ParsedRequest_create();

// Declares a function ParsedRequest_parse that takes a pointer to a ParsedRequest, a request buffer (buf), and its length (buflen). 
// It parses the request buffer and stores the parsed data in the ParsedRequest object.
int ParsedRequest_parse(struct ParsedRequest * parse, const char *buf,
			int buflen);

// Declares a function ParsedRequest_destroy that takes a pointer to a ParsedRequest and frees any memory dynamically allocated for it.
void ParsedRequest_destroy(struct ParsedRequest *pr);

// Declares a function ParsedRequest_unparse that takes a ParsedRequest and an allocated buffer (buf). 
// It writes the entire HTTP request (request line and headers) to this buffer.
int ParsedRequest_unparse(struct ParsedRequest *pr, char *buf, 
			  size_t buflen);

// Declares a function ParsedRequest_unparse_headers that writes only the headers (not the request line) from the ParsedRequest object to the provided buffer.
int ParsedRequest_unparse_headers(struct ParsedRequest *pr, char *buf, 
				  size_t buflen);

// Declares a function ParsedRequest_totalLen that returns the total length of the parsed HTTP request, including the request line, headers, and trailing \r\n.
size_t ParsedRequest_totalLen(struct ParsedRequest *pr);

// Declares a function ParsedHeader_headersLen that returns the length of the headers (excluding the request line) in the parsed request, including the trailing \r\n.
size_t ParsedHeader_headersLen(struct ParsedRequest *pr);

// Declares a function ParsedHeader_set that sets the value of a specific header in the parsed request. 
// It takes the ParsedRequest, a header key, and its associated value.
int ParsedHeader_set(struct ParsedRequest *pr, const char * key, 
		      const char * value);

// Declares a function ParsedHeader_get that retrieves a header by its key from the parsed request and returns a pointer to the corresponding ParsedHeader structure.
struct ParsedHeader* ParsedHeader_get(struct ParsedRequest *pr, 
				      const char * key);

// Declares a function ParsedHeader_remove that removes a header by its key from the parsed request.
int ParsedHeader_remove (struct ParsedRequest *pr, const char * key);

// Declares a debug function that takes a format string and a variable number of arguments. 
// If DEBUG is set to 1, this function prints debugging information to standard output.
void debug(const char * format, ...);

/* Example usage:

   const char *c = 
   "GET http://www.google.com:80/index.html/ HTTP/1.0\r\nContent-Length:"
   " 80\r\nIf-Modified-Since: Sat, 29 Oct 1994 19:43:31 GMT\r\n\r\n";
   
   int len = strlen(c); 
   //Create a ParsedRequest to use. This ParsedRequest
   //is dynamically allocated.
   ParsedRequest *req = ParsedRequest_create();
   if (ParsedRequest_parse(req, c, len) < 0) {
       printf("parse failed\n");
       return -1;
   }

   printf("Method:%s\n", req->method);
   printf("Host:%s\n", req->host);

   // Turn ParsedRequest into a string. 
   // Friendly reminder: Be sure that you need to
   // dynamically allocate string and if you
   // do, remember to free it when you are done.
   // (Dynamic allocation wasn't necessary here,
   // but it was used as an example.)
   int rlen = ParsedRequest_totalLen(req);
   char *b = (char *)malloc(rlen+1);
   if (ParsedRequest_unparse(req, b, rlen) < 0) {
      printf("unparse failed\n");
      return -1;
   }
   b[rlen]='\0';
   // print out b for text request 
   free(b);
  

   // Turn the headers from the request into a string.
   rlen = ParsedHeader_headersLen(req);
   char buf[rlen+1];
   if (ParsedRequest_unparse_headers(req, buf, rlen) < 0) {
      printf("unparse failed\n");
      return -1;
   }
   buf[rlen] ='\0';
   //print out buf for text headers only 

   // Get a specific header (key) from the headers. A key is a header field 
   // such as "If-Modified-Since" which is followed by ":"
   struct ParsedHeader *r = ParsedHeader_get(req, "If-Modified-Since");
   printf("Modified value: %s\n", r->value);
   
   // Remove a specific header by name. In this case remove
   // the "If-Modified-Since" header. 
   if (ParsedHeader_remove(req, "If-Modified-Since") < 0){
      printf("remove header key not work\n");
     return -1;
   }

   // Set a specific header (key) to a value. In this case,
   //we set the "Last-Modified" key to be set to have as 
   //value  a date in February 2014 
   
    if (ParsedHeader_set(req, "Last-Modified", " Wed, 12 Feb 2014 12:43:31 GMT") < 0){
     printf("set header key not work\n");
     return -1;

    }

   // Check the modified Header key value pair
    r = ParsedHeader_get(req, "Last-Modified");
    printf("Last-Modified value: %s\n", r->value);

   // Call destroy on any ParsedRequests that you
   // create once you are done using them. This will
   // free memory dynamically allocated by the proxy_parse library. 
   ParsedRequest_destroy(req);
*/

#endif
