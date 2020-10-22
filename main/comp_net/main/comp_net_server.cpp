/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <chrono>
#include "utilities/time_utility.h"
#include "asteroid_net/network_server.h"


int main(int argc, char** argv)
{
    unsigned short port = 0;
    if(argc == 2)
    {
        std::string portArg = argv[1];
        port = std::stoi(portArg);
    }
    neko::net::ServerNetworkManager server;
    if(port != 0)
    {
        server.SetTcpPort(port);
    }
    server.Init();
    auto clock = std::chrono::system_clock::now();
    while(server.IsOpen())
    {
        const auto start = std::chrono::system_clock::now();
        const auto dt = std::chrono::duration_cast<neko::seconds>(start - clock);
        clock = start;
        server.Update(dt);
    }
    return 0;
}