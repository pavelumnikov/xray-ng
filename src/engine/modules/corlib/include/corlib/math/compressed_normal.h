#pragma once

#include "corlib/math/vector.h"

namespace xr::math
{

// a Unit sse_vector to 16-bit word conversion algorithm
// based on work of Rafael Baptista (rafael@oroboro.com)
// Accuracy improved by O.D. (punkfloyd@rocketmail.com)

// a compressed unit vector3. reasonable fidelty for unit vectors in a 16 bit
// package. Good enough for surface normals we hope.

uint16_t pv_compress(vector const& vec);
void pv_decompress(vector& vec, uint16_t mVec);

} // namespace xr::math