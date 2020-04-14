#ifndef EASINGS_V2_HPP
#define EASINGS_V2_HPP

namespace tnt
{
    class Easing
    {
      public:
        virtual ~Easing() noexcept {}

        virtual void Run(float time, float begin, float change,
                         float duration) = 0;
    };
} // namespace tnt

#endif //! EASINGS_V2_HPP