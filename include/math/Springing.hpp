#ifndef TNT_NUMERIC_SPRINGING_HPP
#define TNT_NUMERIC_SPRINGING_HPP

#include <cmath>

namespace tnt
{
// thx allenchou
// http://allenchou.net/2015/04/game-math-precise-control-over-numeric-springing/

/*
  x     - value             (input/output)
  v     - velocity          (input/output)
  xt    - target value      (input)
  zeta  - damping ratio     (input)
  omega - angular frequency (input)
  h     - time step         (input)
*/
void Spring(float &x, float &v, float xt,
            float zeta, float omega, float h)
{
    const float f{1.0f + 2.0f * h * zeta * omega};
    const float oo{omega * omega};
    const float hoo{h * oo};
    const float hhoo{h * hoo};
    const float detInv{1.0f / (f + hhoo)};
    const float detX{f * x + h * v + hhoo * xt};
    const float detV{v + hoo * (xt - x)};
    x = detX * detInv;
    v = detV * detInv;
}

// see stands for Semi Implicit Euler
/*
  x     - value             (input/output)
  v     - velocity          (input/output)
  xt    - target value      (input)
  zeta  - damping ratio     (input)
  omega - angular frequency (input)
  h     - time step         (input)
*/
void seeSpring(float &x, float &v, float xt,
               float zeta, float omega, float h)
{
    v += -2.0f * h * zeta * omega * v + h * omega * omega * (xt - x);
    x += h * v;
}

// hl stands for Half Life (NOTE: this function has nothing to do with the game by Valve with the same name).
/*
  x     - value             (input/output)
  v     - velocity          (input/output)
  xt    - target value      (input)
  zeta  - damping ratio     (input)
  omega - angular frequency (input)
  h     - time step         (input)
*/
void hlSpring(float &x, float &v, float xt,
              float omega, float h, float lambda)
{
    const float zeta{-log(0.5f) / (omega * lambda)};
    Spring(x, v, xt, zeta, omega, h);
}

} // namespace tnt

#endif //!TNT_NUMERIC_SPRINGING_HPP