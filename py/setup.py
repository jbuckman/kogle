from distutils.core import setup
from Cython.Build import cythonize
from distutils.extension import Extension
from Cython.Distutils import build_ext

extensions = [
    Extension("cgame_entity", sources=["cgame_entity.pyx"], extra_compile_args=["-O3"], language="c++"),
    Extension("cgame_entity_group", sources=["cgame_entity_group.pyx"], extra_compile_args=["-O3"], language="c++")
]

setup(
    name="cpputil",
    ext_modules = cythonize(extensions),
)