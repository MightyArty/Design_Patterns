#include <chrono>
#include <iostream>
#include <future>
#include <mutex>

void red()
{
    printf("\033[1;31m");
}

std::mutex mtx;

/**
 * @brief Example of thread safe singelton
 * mutex wrapped
 */
class Singelton{
    public:
        static Singelton& getInstance(){
            std::lock_guard<std::mutex> lock(mtx);
            if(!instance){
                instance = new Singelton();
            }
            return *instance;
        }
    private:
        Singelton() = default;
        ~Singelton() = default;
        Singelton(const Singelton&) = delete;
        Singelton& operator = (const Singelton&) = delete;

        static Singelton *instance;
};

Singelton *Singelton::instance = nullptr;
std::chrono::duration<double> getTime(){

    auto begin = std::chrono::system_clock::now();
    for(size_t i = 0 ; i <= 10000000; i++){
        Singelton::getInstance();
    }
    return std::chrono::system_clock::now() - begin;
};

int main(){
    auto first = std::async(std::launch::async,getTime);
    auto second = std::async(std::launch::async,getTime);
    auto third = std::async(std::launch::async,getTime);
    auto fourth = std::async(std::launch::async,getTime);

    auto res = first.get() + second.get() + third.get() + fourth.get();

    red();
    std::cout << res.count() << std::endl;
    return 0;
}