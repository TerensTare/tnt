// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs2/Transformable.hpp"

// angle

tnt::ecs2::RotateComp::RotateComp(float angle_) noexcept : angle{angle_} {}

void tnt::ecs2::RotateComp::setAngle(float angle_) noexcept { angle = angle_; }
float tnt::ecs2::RotateComp::getAngle() const noexcept { return angle; }

void tnt::ecs2::RotateComp::Rotate(float angle_) noexcept { angle += angle_; }

// scale

tnt::ecs2::ScaleComp::ScaleComp(Vector const &v) noexcept : scale{v} {}
tnt::ecs2::ScaleComp::ScaleComp(float x, float y) noexcept : scale{x, y} {}

void tnt::ecs2::ScaleComp::setScale(Vector const &v) noexcept { scale = v; }
tnt::Vector tnt::ecs2::ScaleComp::getScale() const noexcept { return scale; }

void tnt::ecs2::ScaleComp::Scale(Vector const &v) noexcept { scale += v; }

// position

tnt::ecs2::TransformComp::TransformComp(Vector const &v) noexcept : pos{v} {}
tnt::ecs2::TransformComp::TransformComp(float x, float y) noexcept : pos{x, y} {}

void tnt::ecs2::TransformComp::setPosition(Vector const &v) noexcept { pos = v; }
tnt::Vector tnt::ecs2::TransformComp::getPosition() const noexcept { return pos; }

void tnt::ecs2::TransformComp::Transform(Vector const &v) noexcept { pos += v; }
