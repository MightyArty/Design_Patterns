#ifndef __TEST__H__
#define __TEST__H__
#include "doctest.h"
#include "Q1-3_sources/active_object.hpp"
#include "Q1-3_sources/queue.hpp"
#include "Q4_sources/guard.hpp"
#include "Q5_sources/singelton.hpp"
#include "Q6_sources/reactor.hpp"
using namespace active;
#define BUFFSIZE 1024

p_queue q1;
int portNo;
int listenFd;
struct sockaddr_in svrAdd_t, clntAdd_t;
using namespace std;
pthread_t threadA[1];

void *task1(void *dummyPt)
{
   int sock = *((int *)dummyPt);
   int numberThread = *((int *)dummyPt + 1);
   std::cout << "Thread No: " << pthread_self() << " Socket No " << sock << " NumberThread " << numberThread << std::endl;

   while (true)
   {
      char *writer = 0;
      char reader[BUFFSIZE] = {0};
      bzero(reader, BUFFSIZE);
      if (read(sock, reader, BUFFSIZE) == -1)
      {
         puts("error");
      }
      if (strncmp(reader, "PUSH", 4) == 0)
      {
         puts("Pushed");
         p_value value = (p_value)malloc(sizeof(value));
         strcpy(value->str, reader + 5);
         value->sock = sock;
         enQ(value, q1);
      }
      else if (strncmp(reader, "EXIT", 4) == 0)
      {
         write(sock, "succ", 4);
         close(sock);
         std::cout << "\nClosing thread and connection" << std::endl;
         break;
      }
      else
      {
         write(sock, "(-1)", 4);
      }
   }
   return 0;
}
int server()
{

   portNo = htons(3002);
   std::cout << "Port :" << portNo << std::endl;

   // create socket
   listenFd = socket(AF_INET, SOCK_STREAM, 0);

   if (listenFd < 0)
   {
      std::cerr << "Cannot open socket" << std::endl;
      return listenFd;
   }

   bzero((char *)&svrAdd_t, sizeof(svrAdd_t));

   svrAdd_t.sin_family = AF_INET;
   svrAdd_t.sin_addr.s_addr = INADDR_ANY;
   svrAdd_t.sin_port = htons(portNo);

   // bind socket
   if (bind(listenFd, (struct sockaddr *)&svrAdd_t, sizeof(svrAdd_t)) < 0)
   {
      std::cerr << "Cannot bind" << std::endl;
      return 0;
   }

   if (listen(listenFd, 5) == -1)
   {
      printf("\n listen has failed\n");
      return 0;
   }

   return listenFd;
}
void *task2(void *a)
{
   p_queue q = (p_queue)a;
   BLUE;
   cout << "Thread number - " << pthread_self() << " is working!\n";
   RESET;
   sleep(1);
   enQ((void *)"1", q);
   enQ((void *)"2", q);
   enQ((void *)"3", q);
   enQ((void *)"4", q);
   enQ((void *)"5", q);
   return 0;
}
TEST_CASE("Q1-3 Test")
{
   SUBCASE("Queue Test")
   {
      system("setterm -bold on");
      cout << "________________________________ - Q1 - __________________________________" << endl;
      system("setterm -bold off");
      p_queue q = createQ();
      CHECK(q->size == 0);
      CHECK(q->id == 0);
      q->id = 1;
      CHECK(q->id == 1);
      pthread_t thread1[2];
      void *arg = {q};
      pthread_create(&thread1[0], NULL, task2, arg);
      sleep(1);
      pthread_create(&thread1[1], NULL, task2, arg);

      cout << "Try to Dequeue" << endl;
      printf("1. %c \n", *((char *)deQ(q)));
      printf("2. %c \n", *((char *)deQ(q)));
      printf("3. %c \n", *((char *)deQ(q)));
      pthread_join(thread1[0], NULL);
      pthread_join(thread1[1], NULL);
      CHECK(q->size == 7);
   }
   SUBCASE("Active Object Test")
   {
      system("setterm -bold on");
      cout << "________________________________ - Q2 - __________________________________" << endl;
      system("setterm -bold off");
      cout << "Create Queue" << endl;
      p_queue q = createQ();
      q->id = 1;
      CHECK(q->id == 1);
      enQ(nullptr, q);
      CHECK(q->size == 1);
      activeObject *ao1 = activeObject::newAO(q, func1, func2);
      sleep(2);
      p_value v = (p_value)malloc(sizeof(value));
      v->sock = 2;
      strcpy(v->str, "Try");
      enQ(v, q);
      sleep(3);
   }
   SUBCASE("PipeLine Active Object Test")
   {
      system("setterm -bold on");
      cout << "________________________________- Q3 - __________________________________" << endl;
      system("setterm -bold off");

      // Init new Q
      q1 = createQ();
      p_queue q2 = createQ();
      p_queue q3 = createQ();
      // Set name's Q
      q1->id = 1;
      q2->id = 2;
      q3->id = 3;

      enQ(q2, q1);
      enQ(q3, q2);
      enQ(nullptr, q3);

      activeObject *ao1 = activeObject::newAO(q1, ceasar_cipher, enQ);
      activeObject *ao2 = activeObject::newAO(q2, transform, enQ);
      activeObject *ao3 = activeObject::newAO(q3, sendRes, nullptr);
      sleep(1);
      if (server() < 0)
         return;
      sleep(1);
      cout << "Open another terminal and run the command ";
      YELLOW;
      cout << "make client" << endl;
      RESET;
      cout << "Now run the command";
      YELLOW;
      cout << " ./client" << endl;
      RESET;
      cout << "To proceed to the next test should be sent from the client EXIT" << endl;

      std::cout << "Listening" << std::endl;
      socklen_t len = sizeof(clntAdd_t);
      // this is where client connects. svr will hang in this mode until client conn
      int connFd = accept(listenFd, (struct sockaddr *)&clntAdd_t, &len);

      if (connFd < 0)
      {
         std::cerr << "Cannot accept connection" << std::endl;
         return;
      }
      else
      {
         std::cout << "Connection successful" << std::endl;
      }
      int a[]{connFd, 0};
      int error = pthread_create(&threadA[0], NULL, task1, (void *)&a);
      if (error != 0)
         printf("\nThread can't be created :[%s]",
                strerror(error));

      pthread_join(threadA[0], NULL);

      if (q1->size != 0)
         destroyQ(q1);

      activeObject::destroyAO(ao1);
      activeObject::destroyAO(ao2);
      activeObject::destroyAO(ao3);
   }
}
TEST_CASE("Q4 Test")
{
   SUBCASE("Guard Test")
   {
      system("setterm -bold on");
      cout << "________________________________ - Q4 - __________________________________" << endl;
      system("setterm -bold off");
      // guard_Test();
   }
}
TEST_CASE("Q5 Test")
{
   SUBCASE("Singleton Test")
   {

      system("setterm -bold on");
      cout << "________________________________ - Q5 - __________________________________" << endl;
      system("setterm -bold off");
      Test();
   }
}
TEST_CASE("Q6 Test")
{
   SUBCASE("Reactor Test")
   {
      system("setterm -bold on");
      cout << "________________________________ - Q6 - __________________________________" << endl;
      system("setterm -bold off");
   }
}

#endif //!__TEST__H__