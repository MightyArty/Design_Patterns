#include <chrono>
#include <iostream>
#include <future>
#include <mutex>

class Singelton{
    public:
        static Singelton *&Instance();
        static void Destroy();
        void Print();
        
    private:
        Singelton();
        ~Singelton();
        Singelton(const Singelton &sing);
        const Singelton &operator = (const Singelton &sing);
        static Singelton *m_Singelton;
        static std::mutex m_Mutex;
};

Singelton *Singelton::m_Singelton = NULL;
std::mutex Singelton::m_Mutex;

Singelton *&Singelton::Instance(){
    if(m_Singelton == NULL){
        std::unique_lock<std::mutex> lock(m_Mutex);
    }
    return m_Singelton;
}

void Singelton::Destroy(){
    std::unique_lock<std::mutex> lock(m_Mutex);
    if(m_Singelton){
        delete m_Singelton;
        m_Singelton = NULL;
    }
}

void Singelton::Print(){
    // will print the singelton
}

