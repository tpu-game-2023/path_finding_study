#include "board.h"
#include <algorithm>

bool asc(const Mass* o1, const Mass* o2)
{
	return o1->getCost() < o2->getCost();
}

bool Board::find(const Point& start, const Point& goal)
{
	Mass& mass_start = getMass(start);
	Mass& mass_goal = getMass(goal);

	mass_start.setStatus(Mass::START);
	mass_goal.setStatus(Mass::GOAL);

	open_list_.clear();
	open_list_.emplace_back(&mass_start);

	//�I�[�v�����X�g����Ȃ甲����
	while (!open_list_.empty())
	{
		//���݂̃m�[�h���I�[�v�����X�g�̍ł��R�X�g���Ⴂ����
		std::sort(open_list_.begin(), open_list_.end(), asc);
		auto it = open_list_.begin();
		Mass* current = *it;
		if (current->getStatus() == Mass::GOAL)
		{
			//�o�H��WayPoint�ɕύX
			Mass* p = current;
			while (p)
			{
				if (p->getStatus() != Mass::START && p->getStatus() != Mass::GOAL)
					p->setStatus(Mass::WAYPOINT);
				p = p->getParent();
			}
			return true;
		}

		//���݂̃m�[�h���N���[�Y�h���X�g�Ɉڂ�
		open_list_.erase(it);
		current->setListed(Mass::CLOSE);

		//���݂̃m�[�h�̗אڂ���e�m�[�h�𒲂ׂ�
		const Point& pos = current->getPos();
		Point next[4] = { pos.getRight(),pos.getLeft(),pos.getUp(),pos.getDown() };
		for (auto& c : next)
		{
			//�}�b�v�O�Ȃ�X�L�b�v
			if (c.x() < 0 || c.x() >= BOARD_SIZE)continue;
			if (c.y() < 0 || c.y() >= BOARD_SIZE)continue;

			Mass& m = getMass(c);
			//�����ꂩ�̃��X�g�Ɋ܂܂�邩��Q���Ȃ�X�L�b�v
			if (m.isListed(Mass::OPEN) || m.isListed(Mass::CLOSE) || m.getStatus() == Mass::WALL)continue;

			//�I�[�v�����X�g�Ɉڂ��ăR�X�g���v�Z����
			open_list_.emplace_back(&m);
			m.setParent(current, goal);
			m.setListed(Mass::OPEN);
		}
	}

	return false;
}

void Board::show() const
{
	std::cout << std::endl;

	for (int y = 0; y < BOARD_SIZE; y++)
	{
		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++)
		{
			std::cout << "+-";
		}
		std::cout << "+" << std::endl;

		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++)
		{
			std::cout << "|";
			switch (mass_[y][x].getStatus())
			{
			case Mass::BLANK:
				std::cout << " ";
				break;
			case Mass::START:
				std::cout << "S";
				break;
			case Mass::GOAL:
				std::cout << "G";
				break;
			case Mass::WAYPOINT:
				std::cout << "o";
				break;
			case Mass::WALL:
				std::cout << "#";
				break;
			case Mass::WATER:
				std::cout << "~";
				break;
			case Mass::ROAD:
				std::cout << "$";
				break;
			}
		}
		std::cout << "|" << std::endl;
	}

	std::cout << " ";
	for (int x = 0; x < BOARD_SIZE; x++)
	{
		std::cout << "+-";
	}
	std::cout << "+" << std::endl;

}
