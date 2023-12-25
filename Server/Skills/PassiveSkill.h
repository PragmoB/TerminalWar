#pragma once

#include "Skill.h"

/*
 * 패시브 스킬 설계를 어떻게 잡을 지 고민 많이 했는데, 내 ㅈㄴ천잰거같다
 * 왜 고민을 했냐면 플레이어의 능력치를 올리는 역할을 플레이어 객체와 패시브 스킬 객체 중 어느쪽이 맡아야 할 지
 * 설계의 방향을 결정하기가 어려웠기 때문인데,
 * 플레이어 객체에서 분담하면 향후 패시브 스킬 추가하는 등 상황에서의 확장성이 경직되고,
 * (확장성이 경직되는 이유는 패시브 스킬을 추가할 때마다 플레이어 클래스의 코드를 수정해야 하기 때문임)
 * 패시브 스킬 객체에서 분담하면 캡슐화 설계 원칙이 깨져서
 * (이유는 플레이어 객체의 능력치 멤버변수들을 직접 조작해야 하므로)
 * 어느것 하나도 쉽게 선택하기 어려운 골때리는 상황이었슴
 *
 * 
 * 그러다가 지금같은 설계가 번뜩떠올랐음
 * PassiveSkill 클래스에서 능력치의 종류들을 모두 정해놓고,
 * 기본적으로는 모든 능력치의 값을 0으로 셋팅해놓는것임
 * 패시브 스킬을 추가할 때는 PassiveSkill을 상속받아 PassiveSkill의 가상함수를 재정의 해주면 됨
 * 설계를 이렇게 잡으면 플레이어 객체 측에서 능력치를 올리는 방향으로 가도 확장성이 떨어지지 않음
 * 왜냐하면 플레이어 객체에서 패시브 스킬 객체를 다룰 때 (PassiveSkill*) 자료형으로 뭉뚱그려 다룰 수 있기 때문에.
 * 오늘 하루종일 고민했는데 후련하다.
 */
class PassiveSkill : public Skill
{
public:
	PassiveSkill(Player* owner, int level);

	virtual int get_speed_rate();
	virtual int get_defense_rate();
	virtual int get_damage_rate();
	virtual int get_evasion_rate();
	virtual int get_mov_attack_damage();
	virtual int get_mov_attack_range();
};