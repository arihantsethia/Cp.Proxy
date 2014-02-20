Cp.Proxy
========

A tiny lightweight single threaded HTTP proxy server built in C++.

Instructions:
============================

To Compile:
>$ make

To Run:
>$ ./cpproxy <port>

>eg. $ ./cpproxy 5128


To use telnet:
>$ telnet <host_addr> <port>

>eg. $ telnet localhost 5128 *(If testing on your local machine)* 

Important Points to Remember
============================
<br>
Configuring Mozilla Firefox to use HTTP/1.0 instead of HTTP/1.1
---------------------------------------------------------------

Since this proxy is based on HTTP/1.0 you need to configue your browser to use HTTP/1.0 instead of HTTP/1.1

For Mozilla Firefox you can follow the below steps to tweak your browser's behaviour.

Type 'about:config' in the title bar.
In the search/filter bar, type 'network.http.proxy'
You should see three keys: network.http.proxy.keepalive, network.http.proxy.pipelining, and network.http.proxy.version.
Set keepalive to false. Set version to 1.0. Make sure that pipelining is set to false.
Restart Firefox.


How to Run Cp-Proxy
==============

On Mozilla Firefox
---------------------------------------------------------------
After running the proxy server at the given port (say 5128), open a new terminal and type

Goto Preferences> Network > Settings

>Select Manual Proxy Settings. In the text field next to HTTP proxy: 

>Enter the IP address of machine where server is hosted. (127.0.0.1 in case of local machine).

>Leave all the other proxy fields empty.

>Set No-Proxy to default i.e "localhost, 127.0.0.1"

Click OK.
Click Close.

Now in the address-bar type in address of any page that you want to visit.  

On telnet
---------------------------------------------------------------
After running the proxy server at the given port (say 5128), open a new terminal and type

>$ telnet localhost 5128

to connect to proxy server, then type in the following sample request

>GET /cseintranet/ HTTP/1.0

>Host: jatinga.iitg.ernet.in

*two enters to end the request*

Output will be in the terminal

Few More Point
===============

This proxy server doesn't handle any other method of request other than HTTP GET Request.
In case of any other type of method proxy server will return 
>"501 Method Not Implemented"

While trying from telnet the request must be of proper format to recieve the correct data.
In case of improperly formated request server will return 
>"400 Bad Request"

Contact
=======
Contact:
>Arihant Sethia - +91 - 8011244745 *(incase of problem in compiling/running)*