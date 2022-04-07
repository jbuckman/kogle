
cdef class CGameEntity:

    cdef public int _width
    cdef public int _height
    cdef public float _x
    cdef public float _y
    cdef public int _color
    cdef public int _id

    cpdef int collide(self, CGameEntity other)
