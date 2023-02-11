/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: May 15, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef CONSTITERATOR_H
#define CONSTITERATOR_H

// reference on what needs to be implemented for STL iterators: http://stackoverflow.com/questions/7758580/writing-your-own-stl-container/7759622#7759622

//#include <iostream>
//#include <iterator>

class ConstIterator : public std::iterator<std::random_access_iterator_tag,
		Scalar, int, Scalar*, Scalar&> {
public:
	ConstIterator(int i, int j, const Derived& mat)
	: i_(i), j_(j), rows_(mat.rows()), p_mat_(&mat) {
	}

	ConstIterator(const Iterator& it)
	: i_(it.i_),
	  j_(it.j_),
	  rows_(it.rows_),
	  p_mat_(it.p_mat_) {
	}

	ConstIterator operator+(int i) {
		int posP, iP, jP;
		posP = pos();
		posP += i;
		posToIn(posP, iP, jP);
		return ConstIterator(iP, jP, *p_mat_);
	}

	ConstIterator operator+=(int i) {
		posToIn(pos() + i, i_, j_);
		return *this;
	}

	ConstIterator operator-=(int i) {
		posToIn(pos() - i, i_, j_);
		return *this;
	}

	ConstIterator operator-(int i) {
		int posP, iP, jP;
		posP = pos();
		posP -= i;
		posToIn(posP, iP, jP);
		return ConstIterator(iP, jP, *p_mat_);
	}

	int operator-(const ConstIterator& it) {
		return pos() - it.pos();
	}

	bool operator<(const ConstIterator& it) {
		if (j_ < it.j_) {
			return true;
		} else if (j_ > it.j_) {
			return false;
		} else if (i_ < it.i_) // in this case, j_ == it.j_, hence comparison must be done on i_
				{
			return true;
		}

		return false;
	}

	bool operator==(const ConstIterator& it) {
		if (i_ == it.i_ && j_ == it.j_)
			return true;
		else
			return false;
	}

	bool operator>(const ConstIterator& it) {
		if (j_ > it.j_) {
			return true;
		} else if (j_ > it.j_) {
			return false;
		} else if (i_ > it.i_) // in this case, j_ == it.j_, hence comparison must be done on i_
				{
			return true;
		}

		return false;
	}

	bool operator>=(const ConstIterator& it) {
		if (!(*this < it)) {
			return true;
		}

		return false;
	}

	bool operator!=(const ConstIterator& it) {
		if (i_ != it.i_ || j_ != it.j_)
			return true;
		else
			return false;
	}

	/**
	 * returns const Scalar instead of const Scalar& to avoid error with temporaries
	 */
	const Scalar& operator*() const {
		return (*p_mat_)(i_, j_);
	}

	const Scalar* operator->() const {
		return &(*p_mat_)(i_, j_);
	}

	const ConstIterator& operator++() {
		if (i_ < rows_ - 1) // row increment
				{
			++i_;
		} else // column increment
		{
			i_ = 0;
			++j_;
		}
		return *this;
	}

	const ConstIterator operator++(int) {
		ConstIterator temp(*this);
		if (i_ < rows_ - 1) // row increment
				{
			++i_;
		} else // column increment
		{
			i_ = 0;
			++j_;
		}
		return temp;
	}

	const ConstIterator& operator--() {
		if (i_ > 0) {
			--i_;
		} else {
			i_ = rows_ - 1;
			--j_;
		}
		return *this;
	}

	int i_;
	int j_;
	int rows_;
	const Derived* p_mat_;

	void posToIn(int pos, int& i, int& j) const {
		std::div_t divresult;
		divresult = std::div(pos, rows_);

		i = divresult.rem;
		j = divresult.quot;
	}

	int pos() const {
		return j_ * rows_ + i_;
	}
};

#endif // CONSTITERATOR_H
