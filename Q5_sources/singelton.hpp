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
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        static T *Instance();
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        static void destroy();

    private:
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        MapFile(MapFile const &) = delete;
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        MapFile &operator=(MapFile const &) = delete;

    protected:
        static T *m_instance;
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        MapFile();
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        ~MapFile();
    };

}

#endif