#ifndef YAML_TYPES_HPP
#define YAML_TYPES_HPP

#include <yaml-cpp/yaml.h>
#include "Vector.hpp"

namespace YAML
{
template <>
struct convert<tnt::Vector>
{
	static Node encode(const tnt::Vector &rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		return node;
	}

	static bool decode(const Node &node, tnt::Vector &rhs)
	{
		if (!node.IsSequence() || node.size() != 2)
			return false;
		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		return true;
	}
};
} // namespace YAML

YAML::Emitter &operator<<(YAML::Emitter &out, const tnt::Vector &v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

#endif //!YAML_TYPES_HPP