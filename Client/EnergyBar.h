#pragma once

class EnergyBar
{
private:
	int energy = 0;
	int level = 0;
	const int max_level;
public:
	EnergyBar(int max_level);
	void earn_energy(int amount);
	void hunt_player(int player_level);
	void appear();
	void clear();

	int get_level() const;
};