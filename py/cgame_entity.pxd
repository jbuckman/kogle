from libcpp cimport bool
from libc.stdint cimport uint8_t

cdef class CGameEntity:

    cdef public int _width
    cdef public int _height
    cdef public float _x
    cdef public float _y
    cdef public uint8_t _color
    cdef public int _id

    cdef public bool _isAlive

    cdef public float _vx
    cdef public float _vy

    cdef public uint8_t[:,:] _sprite
    cdef public bool _isSprite

    cpdef void  update(self)
    cpdef bool  collide(self, CGameEntity other)
    
