///////////////////////////////////////////////////////////////////////////////
//
// http://github.com/breese/aware
//
// Copyright (C) 2013 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <new> // std::nothrow
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <avahi-common/timeval.h>
#include <aware/detail/native_socket.hpp>
#include <aware/avahi/detail/poller.hpp>

//-----------------------------------------------------------------------------
// AvahiWatch
//-----------------------------------------------------------------------------

// This struct must be in global namespace
struct AvahiWatch
{
    AvahiWatch(boost::asio::io_service& io,
               int fd,
               AvahiWatchEvent event,
               AvahiWatchCallback callback,
               void *userdata)
        : socket(io, fd),
          callback(callback),
          userdata(userdata),
          revents(AvahiWatchEvent(0))
    {
        if (event & AVAHI_WATCH_IN)
        {
            start_read();
        }
        if (event & AVAHI_WATCH_OUT)
        {
            start_write();
        }
    }

    void update(AvahiWatchEvent /* event */)
    {
        // FIXME
        assert(false);
    }

    AvahiWatchEvent get_events() const
    {
        return revents;
    }

    void start_read()
    {
        revents = (AvahiWatchEvent)(revents & ~AVAHI_WATCH_IN);
        socket.async_read_event(boost::bind(&AvahiWatch::process_read,
                                            this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }

    void process_read(const boost::system::error_code& error,
                      std::size_t /* bytes_transferred */)
    {
        if (!error)
        {
            revents = (AvahiWatchEvent)(revents | AVAHI_WATCH_IN);
            callback(this, socket.native_handle(), AVAHI_WATCH_IN, userdata);
            start_read();
        }
        else if (error == boost::asio::error::operation_aborted)
        {
            // We are closing down
        }
        else
        {
            // FIXME
        }
    }

    void start_write()
    {
        revents = (AvahiWatchEvent)(revents & ~AVAHI_WATCH_OUT);
        socket.async_write_event(boost::bind(&AvahiWatch::process_write,
                                             this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
    }

    void process_write(const boost::system::error_code& error,
                       std::size_t /* bytes_transferred */)
    {
        if (!error)
        {
            revents = (AvahiWatchEvent)(revents | AVAHI_WATCH_OUT);
            callback(this, socket.native_handle(), AVAHI_WATCH_OUT, userdata);
            start_write();
        }
        else if (error == boost::asio::error::operation_aborted)
        {
            // We are closing down
        }
        else
        {
            // FIXME
        }
    }

    aware::detail::native_socket socket;
    AvahiWatchCallback callback;
    void *userdata;
    AvahiWatchEvent revents;
};

extern "C"
AvahiWatch *aware_avahi_watch_new(const AvahiPoll *poll,
                                  int fd,
                                  AvahiWatchEvent event,
                                  AvahiWatchCallback callback,
                                  void *userdata)
{
    const aware::avahi::detail::poller *poller = static_cast<const aware::avahi::detail::poller *>(poll);
    return new (std::nothrow) AvahiWatch(poller->get_io_service(), fd, event, callback, userdata);
}

extern "C"
void aware_avahi_watch_free(AvahiWatch *self)
{
    delete self;
}

extern "C"
void aware_avahi_watch_update(AvahiWatch *self,
                              AvahiWatchEvent event)
{
    self->update(event);
}

extern "C"
AvahiWatchEvent aware_avahi_watch_get_events(AvahiWatch *self)
{
    return self->get_events();
}

//-----------------------------------------------------------------------------
// AvahiTimeout
//-----------------------------------------------------------------------------

class avahi_timer;

// This struct must be in global namespace
struct AvahiTimeout
{
    AvahiTimeout(boost::shared_ptr<avahi_timer>);
    void update(const struct timeval *);
    void release();

    // AvahiTimeout may be deleted before the timer callback is invoked, so the
    // timer is kept as a shared_ptr, for which this class acts as a simple
    // wrapper.
    boost::shared_ptr<avahi_timer> timer;
};

class avahi_timer : public boost::enable_shared_from_this<avahi_timer>
{
public:
    static boost::shared_ptr<avahi_timer> create(boost::asio::io_service& io,
                                                 const struct timeval *tv,
                                                 AvahiTimeoutCallback callback,
                                                 void *userdata)
    {
        boost::shared_ptr<avahi_timer> result = boost::shared_ptr<avahi_timer>(new avahi_timer(io, callback, userdata));
        if (tv)
        {
            result->update(tv);
        }
        result->set_resource(new (std::nothrow) AvahiTimeout(result));
        return result;
    }

    avahi_timer(boost::asio::io_service& io,
                AvahiTimeoutCallback callback,
                void *userdata)
        : resource(0),
          timer(io),
          callback(callback),
          userdata(userdata)
    {
    }

    ~avahi_timer()
    {
        delete resource;
    }

    AvahiTimeout *get_resource() const
    {
        assert(resource);
        return resource;
    }

    void set_resource(AvahiTimeout *res)
    {
        resource = res;
    }

    void cancel()
    {
        boost::system::error_code dummy;
        timer.cancel(dummy);
    }

    void update(const struct timeval *tv)
    {
        if (tv)
        {
            const AvahiUsec deadline = ((tv->tv_sec == 0) && (tv->tv_usec == 0)) ? 0 : -avahi_age(tv);
            boost::system::error_code dummy;
            timer.expires_from_now(boost::posix_time::microseconds(deadline), dummy);
            timer.async_wait(boost::bind(&avahi_timer::process_timeout,
                                         shared_from_this(),
                                         boost::asio::placeholders::error));
        }
        else
        {
            cancel();
        }
    }

    void process_timeout(const boost::system::error_code& error)
    {
        if (!error)
        {
            callback(resource, userdata);
        }
        else if (error == boost::asio::error::operation_aborted)
        {
            // We are closing down
        }
        else
        {
            // FIXME
        }
    }

    AvahiTimeout *resource;
    boost::asio::deadline_timer timer;
    AvahiTimeoutCallback callback;
    void *userdata;
};

AvahiTimeout::AvahiTimeout(boost::shared_ptr<avahi_timer> timer)
    : timer(timer)
{
}

void AvahiTimeout::update(const struct timeval *tv)
{
    timer->update(tv);
}

void AvahiTimeout::release()
{
    timer->cancel();
    timer.reset();
}

extern "C"
AvahiTimeout *aware_avahi_timeout_new(const AvahiPoll *poll,
                                      const struct timeval *tv,
                                      AvahiTimeoutCallback callback,
                                      void *userdata)
{
    const aware::avahi::detail::poller *poller = static_cast<const aware::avahi::detail::poller *>(poll);
    boost::shared_ptr<avahi_timer> timer = avahi_timer::create(poller->get_io_service(), tv, callback, userdata);
    return timer->get_resource();
}

extern "C"
void aware_avahi_timeout_free(AvahiTimeout *self)
{
    self->release();
}

extern "C"
void aware_avahi_timeout_update(AvahiTimeout *self,
                          const struct timeval *tv)
{
    self->update(tv);
}

//-----------------------------------------------------------------------------
// poller
//-----------------------------------------------------------------------------

namespace aware
{
namespace avahi
{
namespace detail
{

poller::poller(boost::asio::io_service& io)
    : io(io)
{
    userdata = this;
    watch_new = &aware_avahi_watch_new;
    watch_update = &aware_avahi_watch_update;
    watch_get_events = &aware_avahi_watch_get_events;
    watch_free = &aware_avahi_watch_free;
    timeout_new = &aware_avahi_timeout_new;
    timeout_update = &aware_avahi_timeout_update;
    timeout_free = &aware_avahi_timeout_free;
}

boost::asio::io_service& poller::get_io_service() const
{
    return io;
}

} // namespace detail
} // namespace avahi
} // namespace aware
