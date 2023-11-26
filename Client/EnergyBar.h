#pragma once

class EnergyBar
{
private:
	int energy = 0;
	int level = 0;
public:
	EnergyBar();
	void earn_energy(int amount);
	void hunt_player(int player_level);
	void appear();

	int get_level() const;
};