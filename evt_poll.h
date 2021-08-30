#ifndef _EVT_POLL_H
#define _EVT_POLL_H

#include <sys/poll.h>
#include <sys/eventfd.h>
#include <assert.h>
#include <unistd.h>

class EventPoll {
public:
    void initialize()
    {
        evt_ = eventfd(0, EFD_NONBLOCK);

        //最初にライトリードしておかないと、EPOLLOUTが発生してしまう
        eventfd_t v;
        eventfd_write(evt_, 1);
        eventfd_read(evt_, &v);


    }


    void terminate()
    {
        close(evt_);
    }


    void wait()
    {
        struct pollfd evt = { evt_, POLLIN , 0 };
        auto ret = poll(&evt, 1, -1);
        assert(ret == 1);
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
};


#endif