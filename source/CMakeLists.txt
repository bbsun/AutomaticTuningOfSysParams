cmake_minimum_required(VERSION 2.8)

project(AutomaticTuningOfSysParams)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(MPI REQUIRED)
include_directories(${MPI_CXX_INCLUDE_PATH})

add_executable( run_mpijob run_mpijob.cxx sziLogService.cxx sziMPIWorkerDOMReader.cxx sziDataDOMReader.cxx )
target_link_libraries( run_mpijob ${ITK_LIBRARIES} ${MPI_CXX_LIBRARIES} )

add_executable( run_reg run_reg.cxx sziLogService.cxx )
target_link_libraries( run_reg ${ITK_LIBRARIES} ${MPI_CXX_LIBRARIES} )
