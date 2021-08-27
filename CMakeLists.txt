cmake_minimum_required(VERSION 3.20)


project(synth)


add_executable(synth)


include_directories(include)



FILE(GLOB sources src/*)

target_sources(synth PRIVATE ${sources})


target_compile_options(synth PRIVATE -g -Wall)
#add_compile_options(-g -Wall)

target_link_libraries(synth PRIVATE m pthread SDL2)

#add_compile_options(-g -Wall)
