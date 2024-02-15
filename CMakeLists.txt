cmake_minimum_required(VERSION 3.16.0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

message(CHECK_START "Build Type is ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -fexceptions -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -fexceptions -std=c++14")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g -O0 -Wall -fexceptions -std=c++14")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O0 -Wall -fexceptions -std=c++14")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3 -Wall -fexceptions -std=c++14 -s")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -Wall -fexceptions -std=c++14 -s")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g -O3 -Wall -fexceptions -std=c++14 -s")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O3 -Wall -fexceptions -std=c++14 -s")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3 -Wall -fexceptions -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -Wall -fexceptions -std=c++14")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g -O3 -Wall -fexceptions -std=c++14")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O3 -Wall -fexceptions -std=c++14")
endif()

set(CMAKE_INSTALL_PREFIX "/usr/local")

project(finalcut)

add_definitions(-DF_VERSION="0.9.0")
add_definitions(-DCOMPILE_FINAL_CUT)

option(F_COMPILE_EXAMPLES "Compile examples" off)
option(F_COMPILE_STATIC "Compile finalcut as static library" off)

message(CHECK_START "Compile examples")

if(F_COMPILE_EXAMPLES)
    message(CHECK_PASS "enabled")
else()
    message(CHECK_PASS "disabled")
endif(F_COMPILE_EXAMPLES)

message(CHECK_START "Compile as static library")

if(F_COMPILE_STATIC)
    message(CHECK_PASS "enabled")
else()
    message(CHECK_PASS "disabled")
endif(F_COMPILE_STATIC)

#
# # finalcut source/header files
#
file(GLOB finalcut_SRC
    "final/dialog/*.cpp"
    "final/dialog/*.h"
    "final/font/*.h"
    "final/input/*.cpp"
    "final/input/*.h"
    "final/menu/*.cpp"
    "final/menu/*.h"
    "final/output/*.cpp"
    "final/output/*.h"
    "final/output/tty/*.cpp"
    "final/output/tty/*.h"
    "final/util/*.cpp"
    "final/util/*.h"
    "final/vterm/*.cpp"
    "final/vterm/*.h"
    "final/widget/*.cpp"
    "final/widget/*.h"
    "final/*.cpp"
    "final/*.h"
)

#
# # finalcut project
#
project(finalcut)

find_package(Curses REQUIRED)

if(F_COMPILE_STATIC)
    add_library(finalcut ${finalcut_SRC})
else()
    add_library(finalcut SHARED ${finalcut_SRC})
endif(F_COMPILE_STATIC)

target_include_directories(finalcut PRIVATE .)
target_link_libraries(finalcut PRIVATE ncurses rt gpm)

#
# # example sources
#
if(F_COMPILE_EXAMPLES)
    file(GLOB 7segmentExample_SRC
        "examples/7segment.cpp"
    )
    file(GLOB backgroundColorExample_SRC
        "examples/background-color.cpp"
    )
    file(GLOB busyExample_SRC
        "examples/busy.cpp"
    )
    file(GLOB calculatorExample_SRC
        "examples/calculator.cpp"
    )
    file(GLOB checklistExample_SRC
        "examples/checklist.cpp"
    )
    file(GLOB choiceExample_SRC
        "examples/choice.cpp"
    )
    file(GLOB dialogExample_SRC
        "examples/dialog.cpp"
    )
    file(GLOB eventLogExample_SRC
        "examples/event-log.cpp"
    )
    file(GLOB fullwidthCharacterExample_SRC
        "examples/fullwidth-character.cpp"
    )
    file(GLOB helloExample_SRC
        "examples/hello.cpp"
    )
    file(GLOB highlightTextExample_SRC
        "examples/highlight-text.cpp"
    )
    file(GLOB inputDialogExample_SRC
        "examples/input-dialog.cpp"
    )
    file(GLOB keyboardExample_SRC
        "examples/keyboard.cpp"
    )
    file(GLOB listboxExample_SRC
        "examples/listbox.cpp"
    )
    file(GLOB listviewExample_SRC
        "examples/listview.cpp"
    )
    file(GLOB mandelbrotExample_SRC
        "examples/mandelbrot.cpp"
    )
    file(GLOB menuExample_SRC
        "examples/menu.cpp"
    )
    file(GLOB mouseExample_SRC
        "examples/mouse.cpp"
    )
    file(GLOB optiMoveExample_SRC
        "examples/opti-move.cpp"
    )
    file(GLOB parallaxScrollingExample_SRC
        "examples/parallax-scrolling.cpp"
    )
    file(GLOB rotozoomerExample_SRC
        "examples/rotozoomer.cpp"
    )
    file(GLOB scrollviewExample_SRC
        "examples/scrollview.cpp"
    )
    file(GLOB stringOperationsExample_SRC
        "examples/string-operations.cpp"
    )
    file(GLOB termAttributesExample_SRC
        "examples/term-attributes.cpp"
    )
    file(GLOB termcapExample_SRC
        "examples/termcap.cpp"
    )
    file(GLOB terminalFocusExample_SRC
        "examples/terminal-focus.cpp"
    )
    file(GLOB timerExample_SRC
        "examples/timer.cpp"
    )
    file(GLOB transparentExample_SRC
        "examples/transparent.cpp"
    )
    file(GLOB treeviewExample_SRC
        "examples/treeview.cpp"
    )
    file(GLOB uiExample_SRC
        "examples/ui.cpp"
    )
    file(GLOB watchExample_SRC
        "examples/watch.cpp"
    )
    file(GLOB windowsExample_SRC
        "examples/windows.cpp"
    )
    file(GLOB xpmviewExample_SRC
        "examples/xpmview.cpp"
        "examples/xpmimage.h"
    )
endif(F_COMPILE_EXAMPLES)

#
# # example projects
#
if(F_COMPILE_EXAMPLES)
    #
    # # 7segment example project
    #
    project(7segment)

    add_executable(7segment ${7segmentExample_SRC})
    target_link_libraries(7segment PRIVATE finalcut)
    target_include_directories(7segment PRIVATE .)

    #
    # # background-color example project
    #
    project(background-color)

    add_executable(background-color ${backgroundColorExample_SRC})
    target_link_libraries(background-color PRIVATE finalcut)
    target_include_directories(background-color PRIVATE .)

    #
    # # busy example project
    #
    project(busy)

    add_executable(busy ${busyExample_SRC})
    target_link_libraries(busy PRIVATE finalcut)
    target_include_directories(busy PRIVATE .)

    #
    # # calculator example project
    #
    project(calculator)

    add_executable(calculator ${calculatorExample_SRC})
    target_link_libraries(calculator PRIVATE finalcut)
    target_include_directories(calculator PRIVATE .)

    #
    # # checklist example project
    #
    project(checklist)

    add_executable(checklist ${checklistExample_SRC})
    target_link_libraries(checklist PRIVATE finalcut)
    target_include_directories(checklist PRIVATE .)

    #
    # # choice example project
    #
    project(choice)

    add_executable(choice ${choiceExample_SRC})
    target_link_libraries(choice PRIVATE finalcut)
    target_include_directories(choice PRIVATE .)

    #
    # # dialog example project
    #
    project(dialog)

    add_executable(dialog ${dialogExample_SRC})
    target_link_libraries(dialog PRIVATE finalcut)
    target_include_directories(dialog PRIVATE .)

    #
    # # event-log example project
    #
    project(event-log)

    add_executable(event-log ${eventLogExample_SRC})
    target_link_libraries(event-log PRIVATE finalcut)
    target_include_directories(event-log PRIVATE .)

    #
    # # fullwidth-character project
    #
    project(fullwidth-character)

    add_executable(fullwidth-character ${fullwidthCharacterExample_SRC})
    target_link_libraries(fullwidth-character PRIVATE finalcut)
    target_include_directories(fullwidth-character PRIVATE .)

    #
    # # hello example project
    #
    project(hello)

    add_executable(hello ${helloExample_SRC})
    target_link_libraries(hello PRIVATE finalcut)
    target_include_directories(hello PRIVATE .)

    #
    # # highlight-text example project
    #
    project(highlight-text)

    add_executable(highlight-text ${highlightTextExample_SRC})
    target_link_libraries(highlight-text PRIVATE finalcut)
    target_include_directories(highlight-text PRIVATE .)

    #
    # # input-dialog example project
    #
    project(input-dialog)

    add_executable(input-dialog ${inputDialogExample_SRC})
    target_link_libraries(input-dialog PRIVATE finalcut)
    target_include_directories(input-dialog PRIVATE .)

    #
    # # keyboard example project
    #
    project(keyboard)

    add_executable(keyboard ${keyboardExample_SRC})
    target_link_libraries(keyboard PRIVATE finalcut)
    target_include_directories(keyboard PRIVATE .)

    #
    # # listbox example project
    #
    project(listbox)

    add_executable(listbox ${listboxExample_SRC})
    target_link_libraries(listbox PRIVATE finalcut)
    target_include_directories(listbox PRIVATE .)

    #
    # # listview example project
    #
    project(listview)

    add_executable(listview ${listviewExample_SRC})
    target_link_libraries(listview PRIVATE finalcut)
    target_include_directories(listview PRIVATE .)

    #
    # # mandelbrot example project
    #
    project(mandelbrot)

    add_executable(mandelbrot ${mandelbrotExample_SRC})
    target_link_libraries(mandelbrot PRIVATE finalcut)
    target_include_directories(mandelbrot PRIVATE .)

    #
    # # menu example project
    #
    project(menu)

    add_executable(menu ${menuExample_SRC})
    target_link_libraries(menu PRIVATE finalcut)
    target_include_directories(menu PRIVATE .)

    #
    # # mouse example project
    #
    project(mouse)

    add_executable(mouse ${mouseExample_SRC})
    target_link_libraries(mouse PRIVATE finalcut)
    target_include_directories(mouse PRIVATE .)

    #
    # # opti-move example project
    #
    project(opti-move)

    add_executable(opti-move ${optiMoveExample_SRC})
    target_link_libraries(opti-move PRIVATE finalcut)
    target_include_directories(opti-move PRIVATE .)

    #
    # # parallax-scrolling example project
    #
    project(parallax-scrolling)

    add_executable(parallax-scrolling ${parallaxScrollingExample_SRC})
    target_link_libraries(parallax-scrolling PRIVATE finalcut)
    target_include_directories(parallax-scrolling PRIVATE .)

    #
    # # rotozoomer example project
    #
    project(rotozoomer)

    add_executable(rotozoomer ${rotozoomerExample_SRC})
    target_link_libraries(rotozoomer PRIVATE finalcut)
    target_include_directories(rotozoomer PRIVATE .)

    #
    # # scrollview example project
    #
    project(scrollview)

    add_executable(scrollview ${scrollviewExample_SRC})
    target_link_libraries(scrollview PRIVATE finalcut)
    target_include_directories(scrollview PRIVATE .)

    #
    # # string-operations example project
    #
    project("string-operations")

    add_executable("string-operations" ${stringOperationsExample_SRC})
    target_link_libraries("string-operations" PRIVATE finalcut)
    target_include_directories("string-operations" PRIVATE .)

    #
    # # term-attributes example project
    #
    project(term-attributes)

    add_executable(term-attributes ${termAttributesExample_SRC})
    target_link_libraries(term-attributes PRIVATE finalcut)
    target_include_directories(term-attributes PRIVATE .)

    #
    # # termcap example project
    #
    project(termcap)

    add_executable(termcap ${termcapExample_SRC})
    target_link_libraries(termcap PRIVATE finalcut)
    target_include_directories(termcap PRIVATE .)

    #
    # # terminal-focus example project
    #
    project(terminal-focus)

    add_executable(terminal-focus ${terminalFocusExample_SRC})
    target_link_libraries(terminal-focus PRIVATE finalcut)
    target_include_directories(terminal-focus PRIVATE .)

    #
    # # timer example project
    #
    project(timer)

    add_executable(timer ${timerExample_SRC})
    target_link_libraries(timer PRIVATE finalcut)
    target_include_directories(timer PRIVATE .)

    #
    # # transparent example project
    #
    project(transparent)

    add_executable(transparent ${transparentExample_SRC})
    target_link_libraries(transparent PRIVATE finalcut)
    target_include_directories(transparent PRIVATE .)

    #
    # # treeview example project
    #
    project(treeview)

    add_executable(treeview ${treeviewExample_SRC})
    target_link_libraries(treeview PRIVATE finalcut)
    target_include_directories(treeview PRIVATE .)

    #
    # # ui example project
    #
    project(ui)

    add_executable(ui ${uiExample_SRC})
    target_link_libraries(ui PRIVATE finalcut)
    target_include_directories(ui PRIVATE .)

    #
    # # watch example project
    #
    project(watch)

    add_executable(watch ${watchExample_SRC})
    target_link_libraries(watch PRIVATE finalcut)
    target_include_directories(watch PRIVATE .)

    #
    # # windows example project
    #
    project(windows)

    add_executable(windows ${windowsExample_SRC})
    target_link_libraries(windows PRIVATE finalcut)
    target_include_directories(windows PRIVATE .)

    #
    # # xpmview example project
    #
    project(xpmview)

    add_executable(xpmview ${xpmviewExample_SRC})
    target_link_libraries(xpmview PRIVATE finalcut)
    target_include_directories(xpmview PRIVATE .)
endif(F_COMPILE_EXAMPLES)
