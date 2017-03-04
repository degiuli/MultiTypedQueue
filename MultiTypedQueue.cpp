/*--
The MIT License (MIT)

Copyright (c) 2012-2017 Fabio Lourencao De Giuli (http://degiuli.github.io)
Copyright (c) 1998-2017 De Giuli Informatica Ltda. (http://www.degiuli.com.br)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--*/

#include "MultiTypedQueue.h"
#include <thread>
#include <sstream>
#include <string>
#include <iostream>
#include <atomic>
#include <chrono>

int main()
{
    std::atomic<bool> running{ true };
    MultiTypedQueue<int, double, std::string> mtq;
    MultiTypedQueue<std::string> messages;

    // Create the consumer
    auto consumer = std::thread([&mtq, &messages, &running]()
    {
        while (running.load(std::memory_order_acquire))
        {
            int base{ 0 };
            double value{ 0.0 };
            std::string print{};

            std::tie(base, value, print) = mtq.pop();
            std::stringstream message;
            message << "Consumer: Int = " << base << ", Value = " << value << ", " << print;
            messages.push(message.str());
        }
    });

    // Create the consumer
    auto messages_consumer = std::thread([&messages, &running]()
    {
        while (running.load(std::memory_order_acquire))
        {
            std::string message{};
            std::tie(message) = messages.pop();
            std::cout << message << "\n";
        }
    });

    // Start producing
    for (int x = 1; x <= 50; ++x)
    {
        double value1 = x * 111.1;
        std::string print1;
        std::stringstream ss1;
        ss1 << x << " * 111.1 = " << value1;
        auto message1 = ss1.str();

        double value2 = x * 222.2;
        std::string print2;
        std::stringstream ss2;
        ss2 << x << " * 222.2 = " << value2;
        auto message2 = ss2.str();

        double value3 = x * 333.3;
        std::string print3;
        std::stringstream ss3;
        ss3 << x << " * 333.3 = " << value3;
        auto message3 = ss3.str();

        std::stringstream msg1;
        msg1 << "Producer sending: " << x << ", " << value1 << " and " << message1;
        messages.push(msg1.str());
        mtq.push(x, value1, message1);

        std::stringstream msg2;
        msg2 << "Producer sending: " << x << ", " << value2 << " and " << message2;
        messages.push(msg2.str());
        mtq.push(x, value2, message2);

        std::stringstream msg3;
        msg3 << "Producer sending: " << x << ", " << value3 << " and " << message3;
        messages.push(msg3.str());
        mtq.push(x, value3, message3);

        const int wait = static_cast<int>(value1 / 2);
        std::this_thread::sleep_for(std::chrono::milliseconds(wait));
    }

    //to go out of the pop loop
    running.store(false, std::memory_order_release);
    mtq.push({}, {}, {});
    if (consumer.joinable())
    {
        consumer.join();
    }
    if (messages_consumer.joinable())
    {
        messages_consumer.join();
    }

    std::cout << "Press Return to terminate";
    std::cin.get();
    return 0;
}
