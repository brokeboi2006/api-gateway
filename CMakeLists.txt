cmake_minimum_required(VERSION 3.15)
project(API_GATEWAY VERSION 0.1.0 LANGUAGES CXX)

# 1. Убираем ворнинги, которые ты видел в терминале
if(POLICY CMP0144)
  cmake_policy(SET CMP0144 NEW) # Разрешаем использовать Boost_ROOT
endif()
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW) # Разрешаем старый способ поиска Boost
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. ПОДСКАЗКИ ДЛЯ APPLE SILICON (Пишем именно Boost_ROOT, а не BOOST_ROOT)
set(Boost_ROOT "/opt/homebrew")
set(Boost_NO_BOOST_CMAKE ON) # Это критично для версии 1.90.0

# 3. ПОИСК (Добавили флаг QUIET, чтобы не спамить, если не нашли сразу)
find_package(Boost REQUIRED COMPONENTS system)

# 4. ПРОВЕРКА ДЛЯ ТЕБЯ
message(STATUS "--- DEBUG INFO ---")
message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")

add_executable(gateway 
    src/main.cpp 
)

target_include_directories(gateway PRIVATE 
    include 
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(gateway PRIVATE 
    ${Boost_LIBRARIES}
    pthread
)