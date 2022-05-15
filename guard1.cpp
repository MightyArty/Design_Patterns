#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include <string>

void red()
{
    printf("\033[1;31m");
}
void yellow()
{
    printf("\033[1;33m");
}
void blue()
{
    printf("\033[0;34m");
}
void green()
{
    printf("\033[0;32m");
}
void reset()
{
    printf("\033[0m");
}
void welcome()
{
    red();
    printf("    $$      $$$$$  $$$$$$$$$ $     $         ");
    blue();
    printf(" $$$$$         $$     $$$$$       $$      $  $    \n");
    red();
    printf("   $  $     $   $      $      $   $          ");
    blue();
    printf(" $   $        $  $    $   $      $  $     $ $    \n");
    red();
    printf("  $ -- $    $$$$$      $        $    ");
    yellow();
    printf(" @@@@@@ ");
    blue();
    printf(" $$$$$$$     $ -- $   $$$$$     $ -- $    $$        \n");
    red();
    printf(" $      $   $    $     $        $            ");
    blue();
    printf(" $     $    $      $  $    $   $      $   $ $         \n");
    red();
    printf("$        $  $     $    $        $            ");
    blue();
    printf(" $$$$$$$   $        $ $     $ $        $  $  $       \n");
}

struct Student {
    Student(std::string name) : name(name) {}
    std:: string name;
    std:: vector <std::string> friends;
    std:: mutex mtx;
    std:: string out() const{
        std::string ret = "\033[1;31mStudent " + name + "\033[1;31m has some friends: ";
        for(const auto& frnd : friends)
            ret += frnd + " ";
        return ret;
    }
};

void message(Student &, Student &){
    // waisting time
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void assign_friend(Student &std1, Student &std2){
    static std::mutex io_mutex;
    {
        std:: lock_guard<std::mutex> lock(io_mutex);
        yellow();
        std::cout << std1.name << " and " << std2.name << " are waiting for locks" << std::endl;
    }
    {
        std::scoped_lock lck(std1.mtx, std2.mtx);

        {
            std::lock_guard<std::mutex> lk(io_mutex);
            green();
            std::cout << std1.name << " and " << std2.name << " succsesfully got locks :)" << std::endl;
        }
        std1.friends.push_back(std2.name);
        std2.friends.push_back(std1.name);
    }
    message(std1,std2);
    message(std2,std1);
}

int main(){
    welcome();
    printf("\n");
    Student Artem("Artem");
    Student Barak("Barak");
    Student Angelina("Angelina Jolie");
    Student Gal("Gal Gatod");

    std::vector<std::thread> threads;
    threads.emplace_back(assign_friend, std::ref(Artem), std::ref(Angelina));
    threads.emplace_back(assign_friend, std::ref(Barak), std::ref(Gal));
    threads.emplace_back(assign_friend, std::ref(Artem), std::ref(Gal));
    threads.emplace_back(assign_friend, std::ref(Barak), std::ref(Angelina));

    for(auto &thread: threads)
        thread.join();
    std::cout << Artem.out() << std::endl;
    std::cout << Angelina.out() << std::endl;
    std::cout << Barak.out() << std::endl;
    std::cout << Gal.out() << std::endl;
}