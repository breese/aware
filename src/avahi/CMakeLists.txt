project(aware-avahi)

add_library(aware-backend STATIC
  service.cpp
  monitor_socket.cpp
  announce_socket.cpp
  detail/announcer.cpp
  detail/browser.cpp
  detail/client.cpp
  detail/error.cpp
  detail/poller.cpp
)
