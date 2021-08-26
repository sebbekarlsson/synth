cmake_minimum_required(VERSION 3.20)


project(wav)


add_executable(wav)


include_directories(include)



FILE(GLOB sources src/*)

target_sources(wav PRIVATE ${sources})


#target_compile_options(wav PRIVATE -g -Wall)
#add_compile_options(-g -Wall)

target_link_libraries(wav PRIVATE m pthread)

#add_compile_options(-g -Wall)
