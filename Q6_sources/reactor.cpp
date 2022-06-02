#include "reactor.hpp"
using namespace re;
using re::Reactor;

void *Reactor::newReactor()
{
    return new Reactor();
}
void Reactor::InstallHandler(Reactor *reactor, int fd, void (*func)())
{
    reactor->p_fds[reactor->index].fd = fd;
    reactor->p_fds[reactor->index].events = POLLIN;

    reactor->handlers[reactor->index] = func;

    reactor->index++;
    reactor->fd_count++;

    reactor->p_fds = (pollfd *)realloc(reactor->p_fds, (reactor->index) * sizeof(struct pollfd));
    reactor->handlers = (void (**)())realloc(reactor->handlers, (reactor->index) * sizeof(void *));
}
void Reactor::RemoveHandler(Reactor *reactor)
{
}
void Reactor::run()
{
    while (!stop)
    {

        int poll_count = poll(p_fds, fd_count, 2500);
        if (poll_count == -1)
        {
            perror("poll");
            exit(1);
        }
        for (int i = 0; i < fd_count; i++)
        {

            int pollin_happened = p_fds[i].revents & POLLIN;
            if (pollin_happened)
            {
                int fd = p_fds[i].fd;
                if (fd == p_fds[0].fd)
                {
                    int newfd = acceptType(handlers[0])(fd, (struct sockaddr *)&clntAdd, &len);
                    if (newfd < 0)
                    {
                        std::cerr << "Cannot accept connection" << std::endl;
                        continue;
                    }
                    else
                    {
                        std::cout << "Connection successful" << std::endl;
                        Reactor::InstallHandler(this, newfd, (void (*)())read);
                    }
                }
                else
                {
                    char buf[BUFFSIZE] = {0};
                    int nbytes = recvType(handlers[i])(fd, buf, (size_t)BUFFSIZE);
                    int sender_fd = p_fds[i].fd;
                    if (nbytes <= 0)
                    {
                        // Got error or connection closed by client
                        if (nbytes == 0)
                        {
                            // Connection closed
                            printf("pollserver: socket %d hung up\n", fd);
                        }
                        else
                        {
                            perror("recv");
                        }

                        close(fd); // Bye!
                    }
                    else
                    {
                        for (int j = 0; j < fd_count; j++)
                        {
                            // Send to everyone!
                            int dest_fd = p_fds[j].fd;

                            // Except the listener and ourselves
                            if (dest_fd != p_fds[0].fd && dest_fd != sender_fd)
                            {
                                if (send(dest_fd, buf, nbytes, 0) == -1)
                                {
                                    perror("send");
                                }
                            }
                        }
                        send(sender_fd, "Send Succsess", 13, 0);
                    }
                }
            }
            else if (p_fds[i].revents & POLLNVAL)
            {
                cout << "fd_count " << p_fds[i].fd << endl;
            }
            else
            {
                printf("Unexpected event occurred: %d\n", p_fds[i].revents);
            }
        }
    }
}
Reactor::Reactor()
{
    p_fds = (struct pollfd *)malloc(sizeof(struct pollfd));
    fd_count = 0;
    fd_size = 5;
    handlers = (void (**)())malloc(sizeof(void *));

    index = 0;
    stop = false;

    mThread = std::thread(&Reactor::run, this);
    mThread.detach();
}
void Reactor::info()
{
    cout << "Server number " << p_fds[0].fd << "Number of connections " << fd_count << endl;
}
Reactor::~Reactor()
{
    if (mThread.joinable())
    {
        mThread.join();
    }
    stop = true;
}
int server(int argc, char *argv[])
{
    int listenFd;
    int portNo;
    if (argc >= 2)
    {
        try
        {
            portNo = atoi(argv[1]);
            if ((portNo > 65535) || (portNo < 2000))
            {
                throw std::invalid_argument("Please enter a port number between 2000 - 65535");
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            portNo = htons(3003);
            std::cout << "Port :" << portNo << std::endl;
        }
    }
    else
    {
        portNo = htons(3003);
        std::cout << "Port :" << portNo << std::endl;
    }

    // create socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenFd < 0)
    {
        std::cerr << "Cannot open socket" << std::endl;
        return listenFd;
    }

    bzero((char *)&svrAdd, sizeof(svrAdd));

    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(portNo);

    // bind socket
    if (bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
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
int main(int argc, char *argv[])
{
    Reactor *r = (Reactor *)Reactor::newReactor();
    Reactor::InstallHandler(r, server(argc, argv), (void (*)())accept);

    sleep(300);
    return 0;
}
