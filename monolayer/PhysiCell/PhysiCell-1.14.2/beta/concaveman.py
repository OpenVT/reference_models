import cffi
import numpy as np

# from scipy.spatial import ConvexHull
import matplotlib.pyplot as plt


_ffi = cffi.FFI()
_ffi.cdef('void pyconcaveman2d(double *points_c, size_t num_points, int *hull_points_c, size_t num_hull_points, double concavity, double lengthThreshold, double **p_concave_points_c, size_t *p_num_concave_points, void (**p_free)(void*));')
#_lib = _ffi.dlopen('/Users/sadaszewski/Documents/workspace/concaveman-cpp/src/main/cpp/libconcaveman.so')
#_lib = _ffi.dlopen('/Users/heiland/git/concaveman-cpp/src/main/cpp/libconcaveman.so')
#_lib = _ffi.dlopen('/Users/heiland/git/concaveman-cpp/src/main/cpp/concaveman.so')
_lib = _ffi.dlopen('/Users/heiland/git/OpenVT/reference_models/monolayer/PhysiCell/PhysiCell-1.14.2/beta/concaveman.so')


#def concaveman2d(points, hull, concavity=2.0, lengthThreshold=0.0):
def concaveman2d( hull, concavity=2.0, lengthThreshold=0.0):
    # points = np.array(points).astype(np.double)
    points = np.loadtxt('pts_378.csv', delimiter=',')

    print("type(points)=",type(points))
    print("points.shape=",points.shape)
    hull = np.array(hull).astype(np.int32)

    if len(points.shape) != 2:
        raise ValueError('points must be a 2-D array')

    if len(hull.shape) != 1:
        raise ValueError('hull must be a 1-D array')

    if np.any(hull >= len(points)) or np.any(hull < 0):
        raise ValueError('hull indices out of bounds')

    p_concave_points_c = _ffi.new('double**')
    p_num_concave_points = _ffi.new('size_t*')
    p_free = _ffi.new('void(**)(void*)')

    points_c = _ffi.cast('double*', points.ctypes.data)
    hull_c = _ffi.cast('int*', hull.ctypes.data)
    _lib.pyconcaveman2d(points_c, len(points),
        hull_c, len(hull),
        concavity, lengthThreshold,
        p_concave_points_c, p_num_concave_points,
        p_free)

    num_concave_points = p_num_concave_points[0]
    print('num_concave_points:', num_concave_points)
    concave_points_c = p_concave_points_c[0]

    buffer = _ffi.buffer(concave_points_c, 8 * 2 * num_concave_points)

    concave_points = np.frombuffer(buffer, dtype=np.double)
    concave_points = concave_points.reshape((num_concave_points, 2))
    concave_points = concave_points.copy()

    print('concave_points:', concave_points)

    p_free[0](concave_points_c)

    return concave_points


if __name__ == '__main__':
#    concaveman2d([[0, 0], [.25, .15], [1, 0], [1, 1]], [0, 2, 3])
    concave_pts = concaveman2d([8948, 9530, 9871, 9742, 9855, 9680, 9973, 8722, 8857, 9358, 9005, 9640, 9935, 9969, 9975, 9744, 9999, 9717, 9987, 9864, 9965, 9994, 9888])
    print("type(concave_pts)= ",type(concave_pts))
    print("concave_pts.shape= ",concave_pts.shape)

    plt.plot(concave_pts[:, 0], concave_pts[:, 1], '-', label='concave_hull') # Plot the original points
    # plt.plot(hull_vertices[:, 0], hull_vertices[:, 1], 'r-', label='Convex Hull') # Plot the hull
    # plt.fill(hull_vertices[:, 0], hull_vertices[:, 1], 'r', alpha=0.3) # Fill the hull
    # plt.legend()
    plt.show()