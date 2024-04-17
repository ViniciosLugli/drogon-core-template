# drogon-core-template

This template utilizes the Drogon framework to create a high-performance C++17 based HTTP application. It is containerized using Docker. The application listens on port 3000 and returns a JSON response when accessed the root path.

## Prerequisites

Before you begin, ensure you have met the following requirements:

-   [Docker](https://docs.docker.com/get-docker/)

## Building and Running using Docker

To build and run the application in a development environment, follow these steps:

1. Clone the repository to your local machine:

    ```sh
    git clone git@github.com:ViniciosLugli/drogon-core-template.git
    cd drogon-core-template
    ```

2. Build and start the container by running:

    ```sh
    docker compose -f docker-compose-dev.yml up --build
    ```

    This command builds the application using the Dockerfile specified in `docker-compose-dev.yml` and starts the service defined under `web`. The application will listen on port 3000, which is mapped to port 3000 on the host.

Once the application is running, you can access it by navigating to `http://localhost:3000` in your web browser.

## Building and Running using CMake

> This section is only relevant if you want to build and run the application without using Docker in a linux environment.

> If you are a developer using vscode, is recommended install locally the dependencies to the linting and debugging tools work properly.

To build and run the application using CMake, first you need to install the dependencies of the Drogon framework:

```sh
sudo apt-get update && apt-get install -y g++ cmake make git libjsoncpp-dev uuid-dev openssl libssl-dev zlib1g-dev
```

Then, follow these steps to build and link the drogon core library on your system:

1. Go to a directory where you want to clone the Drogon repository, for example:

    ```sh
    cd ~/Documents
    ```

2. Clone the repository to your local machine:

    ```sh
    git clone https://github.com/drogonframework/drogon.git
    cd drogon
    ```

3. Build and install the Drogon library:

    ```sh
    git submodule update --init && mkdir build && cd build && cmake .. && make -j $(nproc) && sudo make install
    ```

Now that you have the Drogon library installed on your system, you can build and run the application with using CMake:

1. Create a build directory and navigate to it:

    ```sh
    mkdir -p build && cd build
    ```

2. Generate the build files using CMake:

    ```sh
    cmake .. && make -j $(nproc)
    ```

3. Run the application:

    ```sh
    ./DrogonApp
    ```

## Tutorial of drogon-ctl command and framework

> Note: To use the `drogon_ctl` command, you need to have the Drogon library installed on your system. Follow the steps in the previous section to install the Drogon library before proceeding.

### Testing if the Drogon library is installed

To test if the Drogon library is installed on your system, run the following command:

```sh
drogon_ctl version
```

You should see a message like this:

```sh
     _
  __| |_ __ ___   __ _  ___  _ __
 / _` | '__/ _ \ / _` |/ _ \| '_ \
| (_| | | | (_) | (_| | (_) | | | |
 \__,_|_|  \___/ \__, |\___/|_| |_|
                 |___/

A utility for drogon
Version: 1.9.3
Git commit: 96919df488e0ebaa0ed304bbd76bba33508df3cc
Compilation:
  Compiler: c++
  Compiler ID: GNU
  Compilation flags: -std=c++17 -I/usr/include/jsoncpp -I/usr/local/include
Libraries:
  postgresql: yes  (pipeline mode: yes)
  mariadb: no
  sqlite3: yes
  ssl/tls backend: OpenSSL
  brotli: yes
  hiredis: no
  c-ares: no
  yaml-cpp: no
```

### To create a new controller

Controllers are used to handle incoming HTTP requests, similiar to api endpoints. To create a new controller, first navigate to the controller directory and run create command:

```sh
cd controllers
drogon_ctl create controller <ControllerName>
```

For example, to create a controller named `UserController`, run:

```sh
drogon_ctl create controller UserController
```

By default the name convention is CamelCase, and controllers have the suffix `Controller`. The command will create a new controller file in the `controllers` directory with the content:

UserController.cc:

```cpp
#include "UserController.h"

void UserController::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    // write your application logic here
}
```

UserController.h:

```cpp
#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class UserController : public drogon::HttpSimpleController<UserController>
{
  public:
    void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) override;
    PATH_LIST_BEGIN
    // list path definitions here;
    // PATH_ADD("/path", "filter1", "filter2", HttpMethod1, HttpMethod2...);
    PATH_LIST_END
};
```

In the `UserController.h` file, you can define the paths that the controller will handle. For example, to handle a `GET` request to the `/users` path, add the following line to the `PATH_LIST_BEGIN` and `PATH_LIST_END` block:

```cpp
#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class UserController : public drogon::HttpSimpleController<UserController>
{
  public:
    void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) override;
    PATH_LIST_BEGIN
    PATH_ADD("/users", Get);
    PATH_LIST_END
};
```

Now you can add your application logic to the `asyncHandleHttpRequest` method in the `UserController.cc` file. for example to return a JSON response of a list of users:

```cpp
void UserController::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    Json::Value users;
    users["users"] = Json::arrayValue;
    users["users"].append("Alice");
    users["users"].append("Bob");
    users["users"].append("Charlie");

    auto response = HttpResponse::newHttpJsonResponse(users);
    callback(response);
}
```

When you are done, you can build and run the application using the instructions in the previous section. You can then access the controller using a tool like `curl`

```sh
curl -i http://localhost:3000/users
```

The response should be a JSON array of users:

```json
{
	"users": ["Alice", "Bob", "Charlie"]
}
```

### To create a new filter

Filters are used to intercept incoming HTTP requests and responses, its like a middleware. To create a new filter, first navigate to the filter directory and run create command:

```sh
cd filters
drogon_ctl create filter <FilterName>
```

For example, to create a filter named `AuthFilter`, run:

```sh
drogon_ctl create filter AuthFilter
```

By default the name convention is CamelCase, and filters have the suffix `Filter`. The command will create a new filter file in the `filters` directory with the content:

AuthFilter.cc:

```cpp
#include "AuthFilter.h"

using namespace drogon;

void AuthFilter::doFilter(const HttpRequestPtr &req,
                         FilterCallback &&fcb,
                         FilterChainCallback &&fccb)
{
    //Edit your logic here
    if (1)
    {
        //Passed
        fccb();
        return;
    }
    //Check failed
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setStatusCode(k500InternalServerError);
    fcb(res);
}
```

AuthFilter.h:

```cpp
#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;


class AuthFilter : public HttpFilter<AuthFilter>
{
  public:
    AuthFilter() {}
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override;
};
```

In the `AuthFilter.cc` file, you can define the logic that the filter will execute. For example, to check if the request contains a valid authentication token, you can add the following logic to the `doFilter` method:

```cpp
void AuthFilter::doFilter(const HttpRequestPtr &req,
                         FilterCallback &&fcb,
                         FilterChainCallback &&fccb)
{
    //Edit your logic here
    if (req->getHeader("Authorization") == "Bearer token")
    {
        //Passed
        fccb();
        return;
    }
    //Check failed
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setStatusCode(k401Unauthorized);
    fcb(res);
}
```

Now you can add the filter to the controller by adding it to the `PATH_ADD` macro in the `UserController.h` file:

```cpp
#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class UserController : public drogon::HttpSimpleController<UserController> {
public:
    void asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) override;
    PATH_LIST_BEGIN
    PATH_ADD("/users", Get, "AuthFilter");
    PATH_LIST_END
};
```

When you are done, you can build and run the application using the instructions in the previous section. You can then access the controller using a tool like `curl`:

```sh
curl -i http://localhost:3000/users
```

The response should be a `401 Unauthorized` status code. You can add the `Authorization` header to the request to pass the filter:

```sh
curl -i -H "Authorization: Bearer token" http://localhost:3000/users
```

The response should now be a JSON array of users as before.
