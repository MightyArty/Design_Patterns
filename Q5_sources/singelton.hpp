#ifndef _SINGLETON_H
#define _SINGLETON_H
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "../color.hpp"

using namespace std;
namespace Singelton
{

    template <typename T>
    class MapFile
    {
    public:
        /**
         * @brief create new instace
         */
        static T *Instance();
        /**
         * @brief destroy the instace
         */
        static void destroy();

    private:
        MapFile(MapFile const &) = delete;
        MapFile &operator=(MapFile const &) = delete;

    protected:
        static T *m_instance;
        MapFile();
        ~MapFile();
    };

}
int Test();
using namespace Singelton;
#endif