#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "breakout.h"
namespace py = pybind11;

PYBIND11_MODULE(binding, m) {
  py::class_<Breakout>(m, "Breakout")
    .def(py::init<>())
    .def("reset", &Breakout::reset)
    .def("legalActions", &Breakout::legalActions)
    .def("renderPixels", [](Breakout& breakout, py::buffer b) -> void {
      py::buffer_info info = b.request();
      breakout.renderPixels((uint8_t *)info.ptr);
    })
    .def("step", &Breakout::step);
}