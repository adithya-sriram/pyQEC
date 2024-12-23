SET(CMAKE_CXX_STANDARD 17)

# include pybind11
INCLUDE_DIRECTORIES(../submodules/pybind11/include)

SET(PYBIND11_CPP_STANDARD -std=c++17)
PYBIND11_ADD_MODULE(pyQEC
    pyQEC.cpp
    SimpleTableau.cpp
    PauliString.cpp
    HoneycombLayout.cpp
)

#CONFIGURE_FILE(test.py test.py COPYONLY)

