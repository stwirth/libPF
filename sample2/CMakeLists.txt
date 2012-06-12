
set(QT_USE_QTOPENGL TRUE)

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(sample2_SOURCES main.cpp
    CarMovementModel.cpp
    CarObservationModel.cpp
    CarState.cpp
    CarStateDistribution.cpp
    MainWindow.cpp
    RenderWidget.cpp)

# headers that nead to be moc'ed
set(sample2_HEADERS
    MainWindow.h
    RenderWidget.h)

# ui files
set(sample2_FORMS MainWindowForm.ui)

QT4_WRAP_CPP(sample2_HEADERS_MOC ${sample2_HEADERS})
QT4_WRAP_UI(sample2_HEADERS_FORMS ${sample2_FORMS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(./ ../lib/include)

add_executable(sample2 ${sample2_SOURCES} ${sample2_HEADERS_MOC} ${sample2_HEADERS_FORMS})
target_link_libraries(sample2 ${QT_LIBRARIES} PF)

