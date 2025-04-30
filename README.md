# webserv — HTTP/1.1 Web Server in C++98

## Overview

**webserv** is a fully functional HTTP/1.1 web server implemented in **C++98**, using **non-blocking I/O with `poll()`**.  
It can serve static websites, execute CGI scripts, support multiple ports and virtual hosts, handle large file uploads, and is fully compatible with modern browsers.

This project was built to comply with strict low-level networking and protocol constraints. It operates **with a single event loop**, making it both efficient and scalable.

---

## ✨ Core Features

- ⚡ **Non-blocking, event-driven I/O**
  - Single `poll()` call for all socket read/write operations
  - Properly handles client disconnects and timeouts
  - Zero reads/writes outside of `poll()` monitoring

- 🌐 **HTTP/1.1 Compliance**
  - Supports `GET`, `POST`, and `DELETE` methods
  - Correct handling of headers, status codes, and chunked bodies
  - Customizable default error pages

- 🧰 **Configuration-Based Architecture**
  - NGINX-like config file format
  - Define servers by host/port, server_name, and routing rules
  - Fine-grained control over routes: allowed methods, redirects, root paths, default files, directory listing

- 📦 **CGI Execution**
  - Executes CGI scripts (e.g., PHP, Python)
  - Correct environment handling (`PATH_INFO`, etc.)
  - Handles input/output chunking for POST and CGI responses

- 🗂️ **Static File Hosting + Uploads**
  - Serve complete static sites
  - Upload and save files via POST
  - Auto-directory listing (optional)

- 🔀 **Multiple Ports & Virtual Hosts**
  - Listen on multiple ports and hostnames
  - First matching server becomes default responder for host:port

---

## Build & Run

```bash
make
./webserv webserv.conf
