from distutils.core import setup
from Cython.Build import cythonize
from distutils.extension import Extension
from Cython.Distutils import build_ext

extensions = [
    Extension("py.cgame_entity", sources=["./py/cgame_entity.pyx"], extra_compile_args=["-O3"], language="c++"),
    Extension("py.cgame_entity_group", sources=["./py/cgame_entity_group.pyx"], extra_compile_args=["-O3"], language="c++")
]

setup(
    name="cpputil",
    ext_modules = cythonize(extensions),
)