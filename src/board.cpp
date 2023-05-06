#include "board.h"

//�\�[�g�p�֐�
//�����ɏ����Ȃ��ƃ����J�G���[�����Ȃ�����
bool asc(const Mass* o1, const Mass* o2)
{
	return o1->getCost() < o2->getCost();
}

//�o�H�T�������ăX�e�[�^�X��ݒ肷��
bool Board::find(const Point& start, const Point& goal)
{
	Mass& mass_start = getMass(start);
	Mass& mass_goal = getMass(goal);

	mass_start.setStatus(Mass::START);
	mass_goal.setStatus(Mass::GOAL);

	//�I�[�v�����X�g�ɊJ�n�m�[�h�ǉ�
	open_list_.clear();
	open_list_.push_back(&mass_start);

	while (!open_list_.empty()) //�I�[�v�����X�g����łȂ�
	{
		//���݂̃m�[�h���ł������Ȃ��
		std::sort(open_list_.begin(), open_list_.end(), asc);
		auto it = open_list_.begin();
		Mass* current = *it;
		if (current->getStatus() == Mass::GOAL)//�ړI�n�Ȃ犮��
		{
			Mass* p = current;//�o�H�̃X�e�[�^�X��Mass::WAYPOINT�ɂ���
			while (p)
			{
				if (p->getStatus() == Mass::BLANK)
				{
					p->setStatus(Mass::WAYPOINT);
				}
					p = p->getParent();
			}
			return true;
		}
		else
		{
			//���݂̃m�[�h���N���[�Y���X�g�Ɉړ�
			open_list_.erase(it);
			current->setListed(Mass::CLOSE);
			//���݂̃m�[�h�ɗאڂ���m�[�h�����ׂĒ��ׂ�
			const Point& pos = current->getPos();
			Point next[4] = { pos.getRight(), pos.getLeft(), pos.getUp(), pos.getDown() };
			for (auto& c : next) //�אڃm�[�h
			{
				if (c.x() < 0 || BOARD_SIZE <= c.x()) continue; //�}�b�v�O�Ȃ�X�L�b�v
				if (c.x() < 0 || BOARD_SIZE <= c.y()) continue; //�}�b�v�O�Ȃ�X�L�b�v

				Mass& m = getMass(c);
				if (!m.isListed(Mass::OPEN) && //�I�[�v�����X�g�Ɋ܂܂�Ă��Ȃ�
					!m.isListed(Mass::CLOSE) && //�N���[�Y�h���X�g�Ɋ܂܂�Ă��Ȃ�
					 m.getStatus() != Mass::WALL) //�ǂł͂Ȃ�
				{
					//�I�[�v�����X�g�Ɉڂ��ăR�X�g�v�Z
					open_list_.push_back(&m);
					m.setParent(current, goal);
					m.setListed(Mass::OPEN);
				}
			}
		}
	}

	//�S�[���ɓ��B���Ȃ�����
	return false;
}

//�}�b�v�`��A�ύX�s�v
void Board::show() const 
{
	std::cout << std::endl;

	for (int y = 0; y < BOARD_SIZE; y++) {
		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "+-";
		}
		std::cout << "+" << std::endl;

		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "|";
			switch (mass_[y][x].getStatus()) {
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
	for (int x = 0; x < BOARD_SIZE; x++) {
		std::cout << "+-";
	}
	std::cout << "+" << std::endl;
}
