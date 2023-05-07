#include <algorithm>
#include "board.h"

//�\�[�g�p�̔�r�֐�
bool asc(const Mass* o1, const Mass* o2) {
	return o1->getCost() < o2->getCost();  //o1�̃}�X�ڂ̃R�X�g��o2�̃}�X�ڂ̃R�X�g���r
}

//�ǂ����邩���ׂ�
bool Board::isValidated(const Point &p)
{
	if (getMass(p).getStatus() == Mass::WALL) {  //p�̃}�X�ڂ��ǂ��ǂ���
		return false;
	}
	return true;
}

//�s����ꏊ�̒T��
bool Board::find(const Point &start, const Point &goal)
{
	Mass &mass_start = getMass(start);  //�J�n�n�_�̃}�X�ڂ̏ꏊ��ݒ�
	Mass &mass_goal = getMass(goal);    //�I���n�_�̃}�X�ڂ̏ꏊ��ݒ�

	mass_start.setStatus(Mass::START);    //�J�n�n�_�̃}�X�ڂ�START��ݒu
	mass_goal.setStatus(Mass::GOAL);      //�I���n�_�̃}�X�ڂ�GOAL��ݒu

	//�I�[�v�����X�g�ɊJ�n�m�[�h��ǉ�����
	open_list_.clear();                      //���X�g�̏�����
	open_list_.push_back(&mass_start);  //�J�n�n�_�����X�g�̍Ō�ɒǉ�����B

	while (!open_list_.empty())  //�I�[�v�����X�g����Ȃ�TRUE�A��łȂ����FALSE��Ԃ��A���ꂪFALSE�Ŗ�����ΌJ��Ԃ�
	{
		//���݂̃m�[�h
		std::sort(open_list_.begin(), open_list_.end(), asc);  //�擪�v�f���w���C�e���[�^�ƍŌ���v�f�̃C�e���[�^���r���Đ��̑召�ŕ��ёւ��Ă���
		auto it = open_list_.begin();  //it�Ƀ��X�g�̐擪�v�f����
		Mass* current = *it; //Mass�N���X�ɐ擪�v�f����
		if (current->getStatus() == Mass::GOAL) {  //�}�X�ڂ̒l���S�[����������
			//�ړI�n�Ȃ�o�H�̊���
			Mass* p = current;  //�o�H�X�e�[�^�X��Mass::WAYPOINT�ɂ���
			while (p){
				if (p->getStatus() == Mass::BLANK){
					p->setStatus(Mass::WAYPOINT);
					p = p->getParent();
				}
			}
			return true;
		}
		else {  //�}�X�ڂ̒l���S�[���łȂ����
			//���݂̃m�[�h���N���[�Y���X�g�Ɉڂ�
			open_list_.erase(it);   //it�ɓ����Ă��鐔�̃��X�g�̏ꏊ���폜����B
			current->setListed(Mass::CLOSE);  
			//���݂̃m�[�h�̗אڂ���e�m�[�h�𒲂ׂ�
			const Point &pos = current->getPos();
			Point next[4] = { pos.getRight(),pos.getLeft(),pos.getUp(),pos.getDown() };
			for (auto &c : next) { //�אڃm�[�h
				//�}�b�v�O�Ȃ�X�L�b�v
				if (c.x() < 0 || BOARD_SIZE <= c.x()) continue;  
				if (c.y() < 0 || BOARD_SIZE <= c.y())continue;
				Mass& m = getMass(c);
				if (!m.isListed(Mass::OPEN) &&   //�I�[�v�����X�g�Ɋ܂܂�Ă��Ȃ�
					!m.isListed(Mass::CLOSE) &&  //�N���[�Y���X�g�Ɋ܂܂�Ă��Ȃ�
					m.getStatus() != Mass::WALL) {  //��Q���łȂ�
					//�I�[�v�����X�g�Ɉڂ��ăR�X�g���v�Z����B
					open_list_.push_back(&m);
					m.setParent(current, goal);
					m.setListed(Mass::OPEN);
				}
			}
		}
	}
	return false;
}

//�{�[�h��\��
void Board::show() const 
{
	std::cout << std::endl;

	for (int y = 0; y < BOARD_SIZE; y++) {
		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "+---";
		}
		std::cout << "+" << std::endl;

		//�}�X�ڂ̓�����\��
		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "| ";
			switch (mass_[y][x].getStatus()) {
			case Mass::BLANK:
				std::cout << "  ";
				break;
			case Mass::START:
				std::cout << "S ";
				break;
			case Mass::GOAL:
				std::cout << "G ";
				break;
			case Mass::WAYPOINT:
				std::cout << "o ";
				break;
			case Mass::WALL:
				std::cout << "# ";
				break;
			case Mass::WATER:
				std::cout << "~ ";
				break;
			case Mass::ROAD:
				std::cout << "$ ";
				break;
			}
		}
		std::cout << "|" << std::endl;
	}

	std::cout << " ";
	for (int x = 0; x < BOARD_SIZE; x++) {
		std::cout << "+---";
	}
	std::cout << "+" << std::endl;

}


//mass_[start.y()][start.x()].setStatus(Mass::START);
//mass_[goal.y()][goal.x()].setStatus(Mass::GOAL);

//Point p = start;
//while (p != goal) {
//	if (p != start) mass_[p.x()][p.y()].setStatus(Mass::WAYPOINT);

//	int load_x = abs(p.x() - goal.x());
//	int load_y = abs(p.y() - goal.y());
//	if (load_y < load_x) {
//		if (p.x() < goal.x()) { p.setX(p.x() + 1); continue; }
//		if (goal.x() < p.x()) { p.setX(p.x() - 1); continue; }
//	}
//	if (p.y() < goal.y()) { p.setY(p.y() + 1); continue; }
//	if (goal.y() < p.y()) { p.setY(p.y() - 1); continue; }

//	//if (p.x() < goal.x()) { p.setX(p.x() + 1); continue; }
//	//if (goal.x() < p.x()) { p.setX(p.x() - 1); continue; }
//	//if (p.y() < goal.y()) { p.setY(p.y() + 1); continue; }
//	//if (goal.y() < p.y()) { p.setY(p.y() - 1); continue; }
//}

