cmake_minimum_required(VERSION 3.30)
project(BanatBanking)

set(CMAKE_CXX_STANDARD 20)

# Create auth executable
add_executable(auth
        auth.cpp
        BankAccount.cpp
        BankUtils.cpp
)

# Create user executable
add_executable(user
        user.cpp
        BankAccount.cpp
        BankUtils.cpp
)

# Apply static linking flags to both executables
set_target_properties(auth user PROPERTIES
        LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)