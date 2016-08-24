project(linne)

find_package(OpenAL REQUIRED)

add_library(linne SHARED src/liblinne.cpp)
target_link_libraries(linne ${OPENAL_LIBRARY})
