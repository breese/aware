#ifndef AWARE_BONJOUR_DETAIL_MONITOR_HPP
#define AWARE_BONJOUR_DETAIL_MONITOR_HPP

///////////////////////////////////////////////////////////////////////////////
//
// http://github.com/breese/aware
//
// Copyright (C) 2016 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <queue>
#include <set>
#include <map>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/basic_waitable_timer.hpp>
#include <boost/chrono/system_clocks.hpp>
#include <aware/contact.hpp>
#include <aware/monitor_socket.hpp>
#include <aware/bonjour/detail/handle.hpp>
#include <aware/bonjour/detail/browser.hpp>
#include <aware/bonjour/detail/resolver.hpp>

namespace aware
{
namespace bonjour
{
namespace detail
{

// Bonjour periodically (about once per minute) reports false positives
// where announcements are reported as disappeared only to be reported
// as appeared again shortly after (a second or two.) In reality they
// have never disappeared. Consequently, action on disappear events is
// defered for a couple of seconds.
//
// When Bonjour reports false disappearances they are either reported
// on their actual network interface or on the "any" network interface.
// In the latter case, we cannot know exactly which network interface
// to ignore, so we keep track of the amount of disappearances as well
// as the re-appeared annoucements.

class monitor
    : public browser::listener,
      public resolver::listener
{
public:
    typedef typename aware::monitor_socket::async_listen_handler async_listen_handler;

    monitor(boost::asio::io_service&, detail::handle&);

    void listen(aware::contact&, async_listen_handler);

private:
    // browser::listener
    virtual void on_browser_appear(const aware::contact&, bool);
    virtual void on_browser_disappear(const aware::contact&, bool);
    virtual void on_browser_failure(const boost::system::error_code&);
    // resolver::listener
    virtual void on_resolver_done(const aware::contact&);
    virtual void on_resolver_failure(const boost::system::error_code&);

private:
    boost::asio::io_service& io;
    detail::handle& connection;
    std::string type;
    boost::optional<detail::browser> browser;
    boost::system::error_code permanent_error;

    struct request
    {
        request(aware::contact&, async_listen_handler);

        aware::contact& contact;
        async_listen_handler handler;
    };
    std::queue<request> requests;

    class scope : boost::noncopyable
    {
    public:
        typedef std::set<aware::contact> active_container;
        typedef std::set<aware::contact> addition_container;
        typedef std::multiset<aware::contact> removal_container;

        scope(boost::asio::io_service&, monitor&);
        ~scope();

        void submit_appear(const aware::contact&);
        void commit_appear();
        void submit_disappear(const aware::contact&);
        void commit_disappear();
        void resolved(const aware::contact&);

        void activate(const aware::contact&);
        void deactivate(const aware::contact&);

    private:
        void process_disappear(const boost::system::error_code&);
        void execute_appear();
        void execute_disappear();

    private:
        monitor& self;
        boost::asio::basic_waitable_timer<boost::chrono::steady_clock> timer;
        active_container active;
        addition_container additions;
        removal_container removals;
        std::size_t wildcard_count;
    };
    // Key is name (type is implicit)
    typedef std::map< std::string, boost::shared_ptr<scope> > scope_container;
    scope_container scopes;

    typedef std::set<std::string> name_container;
    name_container uncommitted_appear;
    name_container uncommitted_disappear;

    typedef std::map< aware::contact, boost::shared_ptr<detail::resolver> > resolvers_type;
    resolvers_type resolvers;
};

} // namespace detail
} // namespace bonjour
} // namespace aware

#endif // AWARE_BONJOUR_DETAIL_MONITOR_HPP
