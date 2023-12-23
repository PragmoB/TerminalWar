#include "Skills/Shoot.h"
#include "Skills/Snipe.h"
#include "Skills/Slash.h"
#include "Skills/LightsaberSlash.h"
#include "Skills/ZweihanderSlash.h"
#include "Skills/Wind.h"
#include "Skills/WindSlash.h"
#include "Skills/LightningStrike.h"
#include "Skills//Shoes.h"

const int Shoot::MAX_LEVEL = 5;
const int Shoot::DAMAGE[] = { 100, 105, 110, 115, 120 };
const int Shoot::COOLDOWN[] = { 1200, 1080, 972, 875, 788 };
const int Shoot::BPS[] = { 25, 26, 27, 28, 29 };
const int Shoot::DISTANCE[] = { 10, 11, 12, 13, 14 };

const int Snipe::MAX_LEVEL = 10;
const int Snipe::DAMAGE[] = { 240, 264, 290, 319, 350, 385, 423, 465, 511, 562 };
const int Snipe::COOLDOWN[] = { 2000, 1800, 1620, 1458, 1313, 1182, 1064, 958, 863, 777 };
const int Snipe::BPS[] = { 65, 71, 78, 85, 93, 102, 112, 123, 135, 148 };
const int Snipe::DISTANCE[] = { 25, 27, 29, 31, 34, 37, 40, 44, 48, 52  };

const int Slash::MAX_LEVEL = 5;
const int Slash::DAMAGE[] = { 100, 105, 110, 115, 120 };
const int Slash::COOLDOWN[] = { 1200, 1080, 972, 875, 788 };

const int LightsaberSlash::MAX_LEVEL = 10;
const int LightsaberSlash::DAMAGE[] = { 240, 252, 264, 277, 290, 304, 319, 334, 350, 367 };
const int LightsaberSlash::COOLDOWN[] = { 788, 710, 639, 576, 519, 468, 422, 380, 342, 308 };

const int ZweihanderSlash::MAX_LEVEL = 10;
const int ZweihanderSlash::DAMAGE[] = { 120, 126, 132, 138, 144, 151, 158, 165, 173, 181 };
const int ZweihanderSlash::COOLDOWN[] = { 788, 710, 639, 576, 519, 468, 422, 380, 342, 308 };

const int Wind::MAX_LEVEL = 10;
const int Wind::DAMAGE[] = { 60, 63, 66, 69, 72, 75, 78, 81, 85, 89 };
const int Wind::COOLDOWN[] = { 788, 710, 639, 576, 519, 468, 422, 380, 342, 308 };
const int Wind::BPS[] = { 25, 26, 27, 28, 29, 30, 31, 32, 33, 34 };
const int Wind::DISTANCE[] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

const int WindSlash::MAX_LEVEL = 10;
const int WindSlash::DAMAGE[] = { 120, 126, 132, 138, 144, 151, 158, 165, 173, 181 };
const int WindSlash::COOLDOWN[] = { 788, 710, 639, 576, 519, 468, 422, 380, 342, 308 };

const int LightningStrike::MAX_LEVEL = 7;
const int LightningStrike::DAMAGE[] = { 400, 440, 484, 532, 585, 643, 707 };
const int LightningStrike::COOLDOWN[] = { 30000, 27000, 24300, 21870, 19683, 17715, 15944 };

const int Shoes::MAX_LEVEL = 4;
const int Shoes::SPEED_RATE[] = { 7, 14, 21, 28 };