#ifndef _EVT_EPOLL_H
#define _EVT_EPOLL_H

#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

class EventEPoll {
public:
    void initialize()
    {
        evt_ = eventfd(0, EFD_NONBLOCK);

        //最初にライトリードしておかないと、EPOLLOUTが発生してしまう
        eventfd_t v;
        eventfd_write(evt_, 1);
        eventfd_read(evt_, &v);


        ep_ = epoll_create(1);
        struct epoll_event event = {};
        event.events = EPOLLIN | EPOLLPRI;
        auto ret = epoll_ctl(ep_, EPOLL_CTL_ADD, evt_, &event);
        assert(ret >= 0);
    }

    void terminate()
    {
        close(ep_);
        close(evt_);
    }


    void wait()
    {
        struct epoll_event evt = {};
        while (epoll_wait(ep_, &evt, 1, -1) < 0) {
            assert(errno == EINTR);//debuggerの割り込みとかでも来る
        }
    }

    void signal()
    {
        eventfd_write(evt_, 1);
    }

    void clear()
    {
        eventfd_t v;
        eventfd_read(evt_, &v);
    }

private:
    int evt_ = 0;
    int ep_ = 0;
};


#endif