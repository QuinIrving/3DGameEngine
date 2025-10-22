#pragma once
#include <concepts>
#include <type_traits>
#include "Graphics/VertexIn.h"
#include "Graphics/VertexOut.h"
#include "Graphics/FragmentIn.h"
#include "Graphics/FragmentOut.h"
#include "Math/Mat4.h"

template <typename T>
concept TVertexShader = requires(T shader, const VertexIn& vin, const Mat4<float>& M, const Mat4<float>& V, const Mat4<float>& P) {
	{ shader(vin, M, V, P) } -> std::same_as<VertexOut>;
};

template <typename T>
concept TFragmentShader = requires(T shader, const FragmentIn& fragIn) {
	{ shader(fragIn) } -> std::same_as<FragmentOut>;
};