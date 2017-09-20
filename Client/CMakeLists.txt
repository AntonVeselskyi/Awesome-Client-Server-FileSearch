#min version

cmake_minimum_required (VERSION 3.1.0)




#project name

project (client)







#source files

set(SOURCES SourceC.cpp Client.cpp  SearchFile.cpp SearchFile.h  SearchFile_L.cpp SearchFile_L.h Client.h )





add_executable(client ${SOURCES})



IF (WIN32)

target_link_libraries(client MSVCP140, WS2_32, VCRUNTIME140)

SET( CMAKE_CXX_FLAGS_DEBUG
	"${CMAKE_CXX-FLAGS_DEBUG} /MTd")

SET( CMAKE_CXX_FLAGS_RELEASE
	"${CMAKE_CXX_FLAGS_RELEASE} /MT")



ELSE()


set(THREADS_PREFER_PTHREAD_FLAG ON)


find_package(Threads REQUIRED)


target_link_libraries(client Threads::Threads)






ENDIF()
