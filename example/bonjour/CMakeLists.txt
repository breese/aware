add_executable(bonjour_announce
  bonjour_announce.cpp
)
target_link_libraries(bonjour_announce aware)

add_executable(bonjour_monitor
  bonjour_monitor.cpp
)
target_link_libraries(bonjour_monitor aware)

add_custom_target(bonjour-example DEPENDS bonjour_announce bonjour_monitor)
