#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "breakout.h"
#include "spaceinvaders.h"
#include "seaquest.h"

namespace py = pybind11;

PYBIND11_MODULE(binding, m) {

  py::class_<Breakout>(m, "Breakout")
    .def(py::init<>())
    .def("legalActions", &Breakout::legalActions)
    .def("renderPixels", [](Breakout& breakout, py::buffer b) -> void {
      py::buffer_info info = b.request();
      breakout.renderPixels((uint8_t *)info.ptr);
    })
    .def("step", &Breakout::step)
    .def("setDeltaT",  &Breakout::setDeltaT);


  py::class_<SpaceInvaders>(m, "SpaceInvaders")
    .def(py::init<>())
    .def("legalActions", &SpaceInvaders::legalActions)
    .def("renderPixels", [](SpaceInvaders& SpaceInvaders, py::buffer b) -> void {
      py::buffer_info info = b.request();
      SpaceInvaders.renderPixels((uint8_t *)info.ptr);
    })
<<<<<<< HEAD
    .def("step", &SpaceInvaders::step);
=======
    .def("step", &Spaceinvaders::step)
    .def("setDeltaT", &Spaceinvaders::setDeltaT);
>>>>>>> setDeltaT and dt was added game.h for frame rate independent movement. Pygame implementation of breakout was added

  py::class_<Seaquest>(m, "Seaquest")
    .def(py::init<>())
    .def("legalActions", &Seaquest::legalActions)
    .def("renderPixels", [](Seaquest& seaquest, py::buffer b) -> void {
      py::buffer_info info = b.request();
      seaquest.renderPixels((uint8_t *)info.ptr);
    })
    .def("step", &Seaquest::step)
    .def("setDeltaT", &Seaquest::setDeltaT);
}