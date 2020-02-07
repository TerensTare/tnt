#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <memory>
#include <vector>
#include <algorithm>

#include "physics/Box.hpp"

namespace tnt
{
template <typename T, typename GetBox, typename Equal = std::equal_to<T>, typename Float = float>
class Quadtree
{
    static_assert(
        std::is_convertible_v<std::invoke_result_t<GetBox, T const &>, Box<Float>>,
        "GetBox must be a callable of signature Box<Float>(T const&)");
    static_assert(
        std::is_convertible_v<std::invoke_result_t<Equal, T const &>, Box<Float>>,
        "Equal must be a callable of signature bool(T const&, T const&)");
    static_assert(std::is_arithmetic_v<Float>);

public:
    Quadtree(Box<Float> const &_box, GetBox const &getBox = GetBox(),
             Equal const &equal = Equal())
        : box{_box}, root{std::make_unique<Node>{}},
          getbox{getBox}, isEqual{equal} {}

    Box<Float> computeBox(Box<Float> const &_box, int i) const
    {
        auto origin{_box.topleft()};
        auto childsize{_box.size() / static_cast<Float>(2)};

        if (i == 0) // North West
            return Box<Float>{origin, childsize};
        else if (i == 1) // North East
            return Box<Float>{Vector<Float>{origin.x + childsize.x, origin.y}, childsize};
        else if (i == 2) // South West
            return Box<Float>{Vector<Float>{origin.x, origin.y + childsize.y}, childsize};
        else if (i == 3) // South East
            return Box<Float>{origin + childsize, childsize};
        else
        {
            assert(false && "Invalid child index");
            return Box<Float>{};
        }
    }

    int quadrant(Box<Float> const &node, Box<Float> const &value) const
    {
        auto center{node.center()};
        if (value.right() < center.x) // West
        {
            if (value.bottom() < center.y) // North West
                return 0;
            else if (value.top >= center.y) // South West
                return 2;
            else
                return -1;
        }
        else if (value.left >= center.x) // East
        {
            if (value.bottom() < center.y) // North East
                return 1;
            else if (value.top >= center.y) // South East
                return 3;
            else
                return -1;
        }
        else
            return -1;
    }

    void add(T const &value)
    {
        add_impl(root.get(), 0, box, value);
    }

    void split(Node *node, Box<Float> const &b)
    {
        assert(node != nullptr);
        assert(leaf(node) && "Only leaves can be split");

        for (auto &child : node->children)
            child = std::make_unique<Node>{};
        auto newval{std::vector<T>{}};
    }

private:
    static constexpr auto Threshold{std::size_t{16}};
    static constexpr auto MaxDepth{std::size_t{8}};

    struct Node
    {
        std::array<std::unique_ptr<Node>, 4> children;
        std::vector<T> values;
    };

    void add_impl(Node *node, std::size_t depth, Box<Float> const &b, T const &val)
    {
        assert(node != nullptr);
        assert(b.contains(getbox(val)));

        if (leaf(node))
        {
            if (depth >= MaxDepth || node->values.size() < Threshold)
                node->values.push_back(val);
            else
            {
                split(node, b);
                add_impl(node, depth, b, val);
            }
        }
        else
        {
            auto i{quadrant(box, getbox(val))};
            if (i != -1)
                add_impl(node->children[static_cast<std::size_t>(i)].get(), depth + 1, computeBox(box, i), val);
            else
                node->values.push_back(val);
        }
    }

    bool leaf(Node *node) const
    {
        return node->children.empty();
    }

    Box<Float> box;
    std::unique_ptr<Node> root;
    GetBox getbox;
    Equal isEqual;
};
} // namespace tnt

#endif //!QUADTREE_HPP