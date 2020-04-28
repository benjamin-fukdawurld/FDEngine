# -*- coding: utf-8 -*-
import PyFD3D
import PyFDCore

class PythonBehavior(PyFD3D.Behavior):
    def __init__(self, timeMgr, scene = None, nodeId = None):
        PyFD3D.Behavior.__init__(self, scene, nodeId)
        self.__timeMgr = timeMgr

    def update(self):
        node = self.node.object
        node.entity.eulerAngles = PyFD3D.vec.vec3(0.0, self.__timeMgr.elapsedTime / -2000.0, 0.0)
