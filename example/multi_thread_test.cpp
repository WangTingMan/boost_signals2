#include <iostream>
#include <list>
#include <thread>
#include <vector>
#include <boost_signals2/signals2.hpp>

constexpr int rountine_time = 5000;
boist::signals2::signal<int(int)> sig1;
boist::signals2::signal<int(int)> sig2;

std::mutex s_mutex;
std::list<boist::signals2::connection> s_connections;

int slot_example(int x)
{
    return x;
}

void sleep_rand_time()
{
    std::srand(std::chrono::steady_clock::now().time_since_epoch().count());
    int sleep_t = std::rand() % 20;
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_t));
}

void connect_worker()
{
    for (int i = 0; i < rountine_time; ++i)
    {
        sig1.connect(slot_example);
        sleep_rand_time();
        std::lock_guard locker(s_mutex);
        s_connections.emplace_back(sig1.connect(slot_example));
    }
}

void disconnect_worker()
{
    for (int i = 0; i < rountine_time; ++i)
    {
        sig1.disconnect(slot_example);
        sleep_rand_time();
        std::lock_guard locker(s_mutex);
        if (s_connections.empty())
        {
            continue;
        }
        s_connections.front().disconnect();
        s_connections.pop_front();
    }
}

void invoke_worker()
{
    for (int i = 0; i < rountine_time; ++i)
    {
        sig1(i);
        sleep_rand_time();
    }
}

int main()
{
    std::cout << "test start.\n";
    std::vector<std::thread> worker;
    for (int i = 0; i < 10; ++i)
    {
        worker.push_back(std::thread(connect_worker));
        worker.push_back(std::thread(disconnect_worker));
        worker.push_back(std::thread(invoke_worker));
    }

    for (auto& ele : worker)
    {
        ele.join();
    }

    std::cout << "test done.\n";
    return 0;
}

