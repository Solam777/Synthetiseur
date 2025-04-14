cmake_minimum_required(VERSION 3.30)
project(projet)
set(CMAKE_CXX_STANDARD 23)
# define the location of the header files
include_directories("libraries/imgui/backends"
        "libraries/imgui"
        "libraries/sdl/include"
        "libraries/portaudio/include"
)
add_executable(projet main.cpp
        MainWindow.cpp
        MainWindow.h
        AudioGenerator.cpp
        AudioGenerator.h
        ./libraries/imgui/imgui.cpp
        ./libraries/imgui/imgui_demo.cpp
        ./libraries/imgui/imgui_draw.cpp
        ./libraries/imgui/imgui_widgets.cpp
        ./libraries/imgui/imgui_tables.cpp
        ./libraries/imgui/backends/imgui_impl_sdl3.cpp
        ./libraries/imgui/backends/imgui_impl_sdlrenderer3.cpp
        Oscillateurs.cpp
        Oscillateurs.h
        Enveloppe.cpp
        Enveloppe.h)
if (APPLE)
    set(CMAKE_INSTALL_RPATH
             "${CMAKE_SOURCE_DIR}/../libraries/sdl/lib/macos/SDL3.framework")
    target_link_libraries(projet main.cpp PRIVATE
            "${CMAKE_SOURCE_DIR}/libraries/sdl/lib/macos/SDL3.framework"
            "-framework CoreFoundation")
elseif (WIN32)
    target_link_libraries(projet PRIVATE
            "${CMAKE_SOURCE_DIR}/libraries/sdl/lib/win32/sdl3.dll"
            "${CMAKE_SOURCE_DIR}/libraries/portaudio/lib/win32/portaudio_x64.lib"
    )
    # post-build command to copy the DLLs
    add_custom_command(TARGET projet POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/libraries/sdl/lib/win32/sdl3.dll"
            "${CMAKE_CURRENT_SOURCE_DIR}/libraries/portaudio/lib/win32/portaudio_x64.dll"
            "$<TARGET_FILE_DIR:projet>"
    )
elseif (UNIX)
    target_link_libraries(projet PRIVATE
            "-ldl"
            "-ljack"
             "${CMAKE_SOURCE_DIR}/libraries/sdl/lib/linux-x86_64/libSDL3.a"
    )
endif ()

