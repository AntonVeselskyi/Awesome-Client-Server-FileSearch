#min version

cmake_minimum_required (VERSION 2.8.11)





#project name


project (server)




#source files



set(SOURCES Server.h Server.cpp  SourceS.cpp )





add_executable(server ${SOURCES})








IF (WIN32)


target_link_libraries(server MSVCP140, WS2_32, VCRUNTIME140)
SET( CMAKE_CXX_FLAGS_DEBUG
	"${CMAKE_CXX-FLAGS_DEBUG} /MTd")

SET( CMAKE_CXX_FLAGS_RELEASE
	"${CMAKE_CXX_FLAGS_RELEASE} /MT")



ELSE()



set(THREADS_PREFER_PTHREAD_FLAG ON)


find_package(Threads REQUIRED)


target_link_libraries(server Threads::Threads)





ENDIF()
