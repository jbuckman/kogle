cimport cgame_entity
from cgame_entity cimport CGameEntity
from libc.stdint cimport uint8_t
cimport cython
from cython.view cimport memoryview, array
import numpy as np

cdef class CGameEntityGroup:

    cdef public list _entities
    cdef public int  _count
    cdef public uint8_t[:,:] _defaultBuffer

    def __init__(self):
        self._entities = []
        self._count = 0
        self.zeroDefaultBuffer()

    @cython.boundscheck(False)
    @cython.wraparound(False)
    def setDefaultBuffer(self, uint8_t[:,:] buffer):
        self._defaultBuffer = buffer

    @cython.boundscheck(False)
    @cython.wraparound(False)
    def zeroDefaultBuffer(self):
        self._defaultBuffer = np.ascontiguousarray(np.zeros((64,64), dtype=np.uint8), dtype=np.uint8)

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

    @cython.boundscheck(False)
    @cython.wraparound(False)
    def renderSprite(self, uint8_t[:,:] buffer, uint8_t[:,:] sprite):
        cdef uint8_t* pointer = &buffer[0, 0]
        cdef uint8_t* spritePointer = &sprite[0, 0]
        for j in range(buffer.shape[0]):
            for i in range(buffer.shape[1]):
                buffer[j * 64 + i] =  sprite[j * 64 + i]

    @cython.boundscheck(False)
    @cython.wraparound(False)
    def render(self, uint8_t[:,:] buffer):
        buffer[:] = self._defaultBuffer[:]
        
        for gameEntity in self._entities:
            if gameEntity._isAlive and \
            gameEntity._y >= 0 and gameEntity._y <= 64 - gameEntity._height and \
            gameEntity._x >= 0 and gameEntity._x <= 64 - gameEntity._height:
                
                start_y = int(gameEntity._y)
                end_y   = start_y+gameEntity._height
                start_x = int(gameEntity._x)
                end_x   = start_x+gameEntity._width
                buffer[start_y:end_y,start_x:end_x] = gameEntity._color

