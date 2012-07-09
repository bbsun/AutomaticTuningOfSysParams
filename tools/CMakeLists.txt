project(tools)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(DicomSeriesReadImageWrite2 DicomSeriesReadImageWrite2.cxx )
target_link_libraries(DicomSeriesReadImageWrite2  ${ITK_LIBRARIES})
