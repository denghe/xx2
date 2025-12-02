#pragma once
#include "pch.h"
struct all {
	xx::Frame bg1;
	xx::Frame circle256;
	xx::Frame mouse_pointer;
	xx::Frame pickaxe;
	std::array<xx::Frame, 7> airplane_;
	std::array<xx::Frame, 8> explosion_1_;
	std::array<xx::Frame, 2> minecart_;
	std::array<xx::Frame, 6> monster1_atk_;
	std::array<xx::Frame, 9> monster1_idle_;
	std::array<xx::Frame, 11> monster1_move_;
	std::array<xx::Frame, 28> monster2_atk_;
	std::array<xx::Frame, 32> monster2_idle_;
	std::array<xx::Frame, 45> monster2_move_;
	std::array<xx::Frame, 10> monster3_atk_;
	std::array<xx::Frame, 30> monster3_idle_;
	std::array<xx::Frame, 4> monster3_move_;
	std::array<xx::Frame, 8> monster4_atk_;
	std::array<xx::Frame, 6> monster4_idle_;
	std::array<xx::Frame, 5> monster4_move_;
	std::array<xx::Frame, 6> rock_0_;
	std::array<xx::Frame, 6> rock_1_;
	std::array<xx::Frame, 6> rock_2_;
	std::array<xx::Frame, 6> rock_3_;
	std::array<xx::Frame, 6> rock_4_;
	std::array<xx::Frame, 6> rock_5_;
	std::array<xx::Frame, 6> rock_6_;
	std::array<xx::Frame, 6> rock_7_;
	std::array<xx::Frame, 6> rock_8_;
	std::array<xx::Frame, 3> talent_;

	void Load(std::string rootPath_);
};
