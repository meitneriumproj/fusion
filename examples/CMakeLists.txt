add_executable(fusion_triangle
    triangle/main.cpp
)

target_link_libraries(fusion_triangle
    PRIVATE fusion glfw
)