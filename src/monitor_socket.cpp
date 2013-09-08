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
#include <queue>
#include <utility> // std::pair
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/make_shared.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/asio/placeholders.hpp>
#include <aware/monitor_socket.hpp>
#include <aware/detail/avahi/browser.hpp>

namespace aware
{

namespace detail
{

class monitor
{
    typedef std::pair<boost::system::error_code, aware::contact> response_type;
    typedef aware::monitor_socket::async_listen_handler handler_type;

public:
    monitor(aware::io_service& io,
            const aware::contact& contact)
        : contact(contact)
    {
        browser = boost::make_shared<detail::avahi::browser>(io.get_client(),
                                                             contact,
                                                             boost::bind(&monitor::on_join,
                                                                         this,
                                                                         _1),
                                                             boost::bind(&monitor::on_leave,
                                                                         this,
                                                                         _1),
                                                             boost::bind(&monitor::on_failure,
                                                                         this,
                                                                         _1));
    }

    //! @pre Must be called from an io_service thread
    void prepare(handler_type handler)
    {
        handlers.push(handler);
        perform();
    }

    //! @pre Must be called from an io_service thread
    void perform()
    {
        if (responses.empty())
            return; // Nothing to send
        if (handlers.empty())
            return; // No receiver

        const response_type& response = responses.front();
        handler_type& handler = handlers.front();
        handler(response.first, response.second);
        responses.pop();
        handlers.pop();
    }

    void on_join(const aware::contact& contact)
    {
        boost::system::error_code success;
        responses.push(std::make_pair(success, contact));
        perform();
    }

    void on_leave(const aware::contact& contact)
    {
        boost::system::error_code success;
        responses.push(std::make_pair(success, contact));
        perform();
    }

    void on_failure(const boost::system::error_code& error)
    {
        aware::contact no_contact;
        responses.push(std::make_pair(error, no_contact));
        perform();
    }

private:
    aware::contact contact;
    boost::shared_ptr<detail::avahi::browser> browser;
    std::queue<response_type> responses;
    std::queue<handler_type> handlers;
};

} // namespace detail

monitor_socket::monitor_socket(aware::io_service& io)
    : io(io)
{
}

void monitor_socket::async_listen(const aware::contact& contact,
                                  async_listen_handler handler)
{
    // Perform from io_service thread because the constructor of
    // detail::avahi::browser will invoke the first callback
    io.post(boost::bind(&monitor_socket::do_async_listen,
                        this,
                        contact,
                        handler));
}

void monitor_socket::do_async_listen(const aware::contact& contact,
                                     async_listen_handler handler)
{
    const std::string& key = contact.get_type();
    monitor_map::iterator where = monitors.lower_bound(key);
    if ((where == monitors.end()) || (monitors.key_comp()(key, where->first)))
    {
        where = monitors.insert(where,
                                monitor_map::value_type(key,
                                                        boost::make_shared<aware::detail::monitor>(boost::ref(io), contact)));
    }
    assert(where != monitors.end());
    where->second->prepare(handler);
}

} // namespace aware
