from ctypes import *
pyusedll_lib = CDLL('./mingw_dll.dll')

print(pyusedll_lib)

int1 = c_int(5)
int2 = c_int(6)
pyusedll_lib.add_dll.restype = c_int
summing = pyusedll_lib.add_dll(int1, int2)
print(summing)


int3 = c_int(33)
int4 = c_int(2)
pyusedll_lib.div_dll.restype = None
pyusedll_lib.div_dll(int3, int4)

int5 = c_int(5)
int6 = c_int(9)
result = c_float()
pyusedll_lib.pothag_dll(int5, int6, byref(result))
print(result)