#pragma once
#include "pch.h"
struct _pics {
	std::array<xx::Frame, 6> a_;
	std::array<xx::Frame, 1> bg_;
	std::array<xx::Frame, 6> c_;
	std::array<xx::Frame, 2> f_;
	std::array<xx::Frame, 2> s_;
	std::array<xx::Frame, 7> t_;
	std::array<xx::Frame, 6> w_;

	void Load(std::string rootPath_);
};
