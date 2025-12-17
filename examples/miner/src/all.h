#pragma once
#include "pch.h"
struct all {
	xx::Frame bg1;
	xx::Frame circle256;
	xx::Frame mouse_pointer;
	xx::Frame pickaxe;
	xx::Frame porter1;
	std::array<xx::Frame, 7> airplane_;
	std::array<xx::Frame, 8> explosion_1_;
	std::array<xx::Frame, 2> minecart_;
	std::array<xx::Frame, 6> miner1_atk_;
	std::array<xx::Frame, 9> miner1_idle_;
	std::array<xx::Frame, 11> miner1_move_;
	std::array<xx::Frame, 28> miner2_atk_;
	std::array<xx::Frame, 32> miner2_idle_;
	std::array<xx::Frame, 45> miner2_move_;
	std::array<xx::Frame, 10> miner3_atk_;
	std::array<xx::Frame, 30> miner3_idle_;
	std::array<xx::Frame, 4> miner3_move_;
	std::array<xx::Frame, 8> miner4_atk_;
	std::array<xx::Frame, 6> miner4_idle_;
	std::array<xx::Frame, 5> miner4_move_;
	std::array<xx::Frame, 3> r_;
	std::array<xx::Frame, 5> rock_0_;
	std::array<xx::Frame, 5> rock_1_;
	std::array<xx::Frame, 5> rock_2_;
	std::array<xx::Frame, 5> rock_3_;
	std::array<xx::Frame, 5> rock_4_;
	std::array<xx::Frame, 5> rock_5_;
	std::array<xx::Frame, 5> rock_6_;
	std::array<xx::Frame, 5> rock_7_;
	std::array<xx::Frame, 5> rock_8_;
	std::array<xx::Frame, 3> talent_;

	void Load(std::string rootPath_);
};
