#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
//#include "breakout.h"
#include "spaceinvaders.h"
namespace py = pybind11;

/*
PYBIND11_MODULE(binding, m) {
  py::class_<Breakout>(m, "Breakout")
    .def(py::init<>())
    .def("legalActions", &Breakout::legalActions)
    .def("renderPixels", [](Breakout& breakout, py::buffer b) -> void {
      py::buffer_info info = b.request();
      breakout.renderPixels((uint8_t *)info.ptr);
    })
    .def("step", &Breakout::step);
}
*/

PYBIND11_MODULE(binding, m) {
  py::class_<Spaceinvaders>(m, "Spaceinvaders")
    .def(py::init<>())
    .def("legalActions", &Spaceinvaders::legalActions)
    .def("renderPixels", [](Spaceinvaders& spaceinvaders, py::buffer b) -> void {
      py::buffer_info info = b.request();
      spaceinvaders.renderPixels((uint8_t *)info.ptr);
    })
    .def("step", &Spaceinvaders::step);
}
