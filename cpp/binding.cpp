#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "breakout.h"
#include "spaceinvaders.h"
#include "seaquest.h"
#include "pacman.h"
#include "freeway.h"

namespace py = pybind11;

PYBIND11_MODULE(binding, m) {
  
  py::class_<Breakout>(m, "Breakout")
    .def(py::init<>())
    .def("legalActions", &Breakout::legalActions)
    .def("renderPixels", [](Breakout& breakout, py::buffer b) -> void {
      py::buffer_info info = b.request();
      breakout.renderPixels((uint8_t *)info.ptr);
    })
    .def("step", &Breakout::step);

  py::class_<SpaceInvaders>(m, "SpaceInvaders")
    .def(py::init<>())
    .def("legalActions", &SpaceInvaders::legalActions)
    .def("renderPixels", [](SpaceInvaders& SpaceInvaders, py::buffer b) -> void {
      py::buffer_info info = b.request();
      SpaceInvaders.renderPixels((uint8_t *)info.ptr);
    })
    .def("step", &SpaceInvaders::step);

  py::class_<Seaquest>(m, "Seaquest")
    .def(py::init<>())
    .def("legalActions", &Seaquest::legalActions)
    .def("renderPixels", [](Seaquest& seaquest, py::buffer b) -> void {
      py::buffer_info info = b.request();
      seaquest.renderPixels((uint8_t *)info.ptr);
    })
    .def("step", &Seaquest::step);

  py::class_<Pacman>(m, "Pacman")
    .def(py::init<>())
    .def("legalActions", &Pacman::legalActions)
    .def("renderPixels", [](Pacman& pacman, py::buffer b) -> void {
      py::buffer_info info = b.request();
      pacman.renderPixels((uint8_t *)info.ptr);
    })
    .def("step", &Pacman::step);

  py::class_<Freeway>(m, "Freeway")
    .def(py::init<>())
    .def("legalActions", &Freeway::legalActions)
    .def("renderPixels", [](Freeway& freeway, py::buffer b) -> void {
      py::buffer_info info = b.request();
      freeway.renderPixels((uint8_t *)info.ptr);
    })
    .def("step", &Freeway::step);

}