#include "part3.h"

void cal_part3(double U, double L, double L_1, double l_, double *xs, double *xt, double *dx, struct FACE* head, int mode)
{
	double U_;
	double x, x_, PA, PA_, I, I_, s = 0, t = 0, s_, t_, D, dq, lt_, ls_;
	double n = 1;
	int first = 0;
	struct FACE *ptr;
	ptr = (struct FACE*)malloc(sizeof(struct FACE));
	ptr = head;

	while (ptr != NULL) {
		if (!first) {
			I = asin((L_1 - ptr->r) / ptr->r*sin(U));
			I_ = asin(n / ptr->n[mode] *sin(I));
			U_ = U + I - I_;
			PA = L_1 * sin(U) / cos((I - U) / 2);	//主光线入射到面的PA
			x = pow(PA, 2) / (2 * ptr->r);
			if (L == -INFINITY) {
				s = -INFINITY;
				t = s;
				t_ = ptr->n[mode]*pow(cos(I_), 2) / (ptr->n[mode]*cos(I_) / ptr->r - n * cos(I) / ptr->r);
				s_ = ptr->n[mode] / (ptr->n[mode]*cos(I_) / ptr->r - n * cos(I) / ptr->r);
			}
			else {
				s = (L - x) / cos(U);
				t = s;
				t_ = ptr->n[mode] * pow(cos(I_), 2) / (n*pow(cos(I), 2) / t + ptr->n[mode]*cos(I_) / ptr->r - n * cos(I) / ptr->r);
				s_ = ptr->n[mode] / (n / s + ptr->n[mode]*cos(I_) / ptr->r - n * cos(I) / ptr->r);
			}
			n = ptr->n[mode];
			dq = ptr->d;
			L = L_1 - dq;
			first = 1;

			if (ptr->next == NULL)
				break;
			else
				ptr = ptr->next;
		}
		else {
			U = U_;
			I = asin((L - ptr->r) / ptr->r*sin(U));
			I_ = asin(n / ptr->n[mode]*sin(I));
			U_ = U + I - I_;
			L = ptr->r + ptr->r * (sin(I_) / sin(U_));
			PA_ = L * sin(U) / cos((I - U) / 2); //目前面的PA（用作D的计算）
			x_ = pow(PA_, 2) / (2 * ptr->r);	 //目前面的x（用作D的计算）
			D = (dq - x + x_) / cos(U);			 //前一个面的D（用作计算ti+1和si+1)
			t = t_ - D;							 //目前面的t
			s = s_ - D;							 //目前面的s
			t_ = ptr->n[mode] * pow(cos(I_), 2) / (n*pow(cos(I), 2) / t + ptr->n[mode]*cos(I_) / ptr->r - n * cos(I) / ptr->r);
			s_ = ptr->n[mode] / (n / s + ptr->n[mode]*cos(I_) / ptr->r - n * cos(I) / ptr->r);
			x = x_;								 //将x_还原为目前面的x
			PA = PA_;							 //将PA还原为目前面的PA
			dq = ptr->d;						 //记录dq为后续D计算作准备
			if (ptr->next != NULL) {			 //此时不为最后一个面
				n = ptr->n[mode];
				dq = ptr->d;
				L = L - ptr->d;
				ptr = ptr->next;
			}
			else {								 //当前为最后一个面
				lt_ = t_ * cos(U_) + x;
				ls_ = s_ * cos(U_) + x;
				*xt = lt_ - l_;
				*xs = ls_ - l_;
				*dx = *xt - *xs;
				ptr = ptr->next;
			}

		}
	}
}
