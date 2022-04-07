cimport cgame_entity
from cgame_entity cimport CGameEntity
from libc.stdint cimport uint8_t

cdef class CGameEntityGroup:

    cdef public list _entities
    cdef public int  _count

    def __init__(self):
        self._entities = []
        self._count = 0

    cdef void _append(self, CGameEntity entity):
        entity._id = self._count
        self._count = self._count + 1
        self._entities.append(entity)

    cdef void _remove(self, CGameEntity entity):
        self._entities.remove(entity)

    def remove(self, CGameEntity entity):
        self._remove(entity)

    def add(self, CGameEntity entity):
        self._append(entity)

    def extend(self, list entities):
        for entity in entities:
            self._append(entity)

    cdef void _render(self, uint8_t[:,:] buffer):
        buffer[:] = 0

        for gameEntity in self._entities:

            start_y = int(gameEntity._y)
            end_y   = start_y+gameEntity._height
            start_x = int(gameEntity._x)
            end_x   = start_x+gameEntity._width
            
            buffer[start_y:end_y,start_x:end_x] = gameEntity._color;
    
    def render(self, buffer):
        self._render(buffer)
