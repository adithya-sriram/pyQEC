#include <Python.h>
#include <pybind11/pybind11.h>
#include "HoneycombLayout.h"
#include <pybind11/stl.h>
#include "SimpleTableau.h"

int add(int i, int j) {
    return i + j;
}

namespace py = pybind11;
using namespace pyQEC;
using namespace std;

namespace pyQEC {

}


PYBIND11_MODULE(pyQEC, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function that adds two numbers");

    py::class_<SimpleTableau>(m, "SimpleTableau")
        .def(py::init<int>())
        .def_readwrite("matrix", &SimpleTableau::matrix)
        .def_readonly("m_N", &SimpleTableau::m_N)
        .def_readonly("current_rank", &SimpleTableau::current_rank)
        .def_static("initialize_mixed_on_r_qubits", &SimpleTableau::initialize_mixed_on_r_qubits)
        .def("copy", &SimpleTableau::copy)
        .def("rowReduce", &SimpleTableau::rowReduce)
        .def_static("row_echelon", &SimpleTableau::row_echelon_vector)
        .def("rank", &SimpleTableau::rank)
        .def("measure", &SimpleTableau::measure)
        .def("entanglement_entropy", &SimpleTableau::entanglement_entropy)
        .def("cnot", &SimpleTableau::cnot)
        .def("hadamard", &SimpleTableau::hadamard)
        .def("phase", &SimpleTableau::phase);

    py::class_<PauliString>(m, "PauliString")
        .def(py::init<string>())
        .def_readonly("op", &PauliString::op)
        .def("len", &PauliString::len)
        .def("eq", &PauliString::eq)
        .def("breakString", &PauliString::breakString)
        .def("add", &PauliString::add)
        .def("mult", &PauliString::mult)
        .def("comm", &PauliString::comm);

    py::class_<HoneycombLayout>(m, "HoneycombLayout")
        .def(py::init<int>())
        .def("get_qubit_coords", &HoneycombLayout::get_qubit_coords)
        .def("get_bonds", &HoneycombLayout::get_bonds);
}
