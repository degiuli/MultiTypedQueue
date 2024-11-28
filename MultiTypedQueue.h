/*--
The MIT License (MIT)

Copyright (c) 2012-2024 Fabio Lourencao De Giuli (http://degiuli.github.io)
Copyright (c) 1998-2024 De Giuli Informatica Ltda. (http://www.degiuli.com.br)

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

#include <queue>
#include <tuple>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <type_traits>

class spin_lock
{
public:
    spin_lock()
    {
        m_LockFlag.clear(std::memory_order_release);
    }

    spin_lock(const spin_lock&) = delete;
    spin_lock& operator=(const spin_lock&) = delete;

    inline void lock()
    {
        while (m_LockFlag.test_and_set(std::memory_order_acquire))
        {
            std::this_thread::yield();
        }
    }
    inline void unlock()
    {
        m_LockFlag.clear(std::memory_order_release);
    }
private:
    std::atomic_flag m_LockFlag;
};

template<class... TYPES>
// std::enable_if_t<!std::is_copy_constructible<TYPES...>::value>
class MultiTypedQueue
{
private:

    std::queue<std::tuple<TYPES...>> queue;
    spin_lock push_mutex;
    spin_lock pop_mutex;
    std::condition_variable cv;
    std::mutex cond_mutex;
    std::atomic<bool> flag{ false };

public:

    MultiTypedQueue() = default;

    void push(TYPES... args)
    {
        std::lock_guard<spin_lock> lk(push_mutex);

        queue.push(std::make_tuple<TYPES...>(std::forward<TYPES>(args)...));

        flag.store(true, std::memory_order_release);

        cv.notify_one();
    }

    std::tuple<TYPES...> pop()
    {
        std::lock_guard<spin_lock> lk(pop_mutex);

        while (queue.empty() == true)
        {
            std::unique_lock<std::mutex> cvlk(cond_mutex);
            cv.wait(cvlk, [&] { 
                return flag.load(std::memory_order_acquire);
            });
        }

        auto queued_data = queue.front();
        queue.pop();

        return queued_data;
    }
};
