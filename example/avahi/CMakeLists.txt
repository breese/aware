add_executable(avahi_announce
  avahi_announce.cpp
)
target_link_libraries(avahi_announce aware)

add_executable(avahi_monitor
  avahi_monitor.cpp
)
target_link_libraries(avahi_monitor aware)

add_executable(readme
  readme.cpp
)
target_link_libraries(readme aware)

add_custom_target(avahi-example DEPENDS avahi_announce avahi_monitor readme)
