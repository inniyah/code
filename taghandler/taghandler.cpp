/*
 * Copyright (C) 2007, 2008  Miriam Ruiz <little_miry@yahoo.es>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ONANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "taghandler.h"

#include <iostream>
#include <cassert>

bool FilterTagHandler::Result::CompareAny(const Result &red) const
{
	unsigned int min_size = size;
	if (min_size > red.size) min_size = red.size;
	for (unsigned int i=0; i<min_size; i++)
		if ((data[i] & red.data[i]) != 0) return true;
	return false;
}

bool FilterTagHandler::Result::CompareAll(const Result &green) const
{
	unsigned int min_size = size;
	if (min_size > green.size) min_size = green.size;
	for (unsigned int i=0; i<min_size; i++)
		if ((data[i] & green.data[i]) != green.data[i]) return false;
	if (size >= green.size) return true;
	for (unsigned int j=size; j<green.size; j++)
		if (green.data[j] != 0) return false;
	return true;
}

void FilterTagHandler::Result::Print(std::ostream &out) const
{
	unsigned int tmp_position = 0;
	unsigned int tmp_index = 0;
	for (unsigned int i=0; i<size; i++)
	{
		unsigned int tmp_flag = 1;
		unsigned int tmp_data = data[tmp_index];
		while (tmp_flag) {
			out << ( tmp_data & tmp_flag ? "*" : "-" ) ;
			tmp_flag = tmp_flag << 1;
			tmp_position++;
			if (tmp_position % 8 == 0) out << " " ;
		}
		tmp_index++;
		out << "  " ;
	}
}

FilterTagHandler::FilterTagHandler() : current_position(0), current_flag(1)
{
}

FilterTagHandler::~FilterTagHandler()
{
}

const FilterTagHandler::Element *FilterTagHandler::GetTag(const std::string name)
{
	for (std::vector<Element>::const_iterator i = elements.begin();
			i != elements.end(); ++i)
	{ if (i->name == name) return &(*i); }
	return NULL;
}

void FilterTagHandler::AddTag(const std::string name)
{
	for (std::vector<Element>::const_iterator i = elements.begin();
			i != elements.end(); ++i)
	{ if (i->name == name) return; }

	Element tag(name, current_position, current_flag);
	current_flag = current_flag << 1;
	if (!current_flag)
	{ current_position++; current_flag=1; }
	elements.push_back(tag);
}

bool FilterTagHandler::TagExists(const std::string name)
{
	for (std::vector<Element>::const_iterator i = elements.begin();
			i != elements.end(); ++i)
	{ if (i->name == name) return true; }
	return false;
}

void FilterTagHandler::PrintAll(std::ostream &out) const
{
	for (std::vector<Element>::const_iterator i = elements.begin();
			i != elements.end(); ++i)
	{
		out << i->name << " (" << i->position << ":" << i->flag << ")" << std::endl;
	}
}

void FilterTagHandler::Print(std::ostream &out, Result *result) const
{
	for (std::vector<Element>::const_iterator i = elements.begin();
			i != elements.end(); ++i)
	{
		if (result->CheckAny(i->position, i->flag))
			out << i->name << " ";
	}
	out << std::endl;
}

int main(int argc, const char * const argv[])
{
	FilterTagHandler tags;
	for (unsigned int i=0; i<70; i++)
	{
		tags.AddTag("A");
		tags.AddTag("B");
		tags.AddTag("C");
		tags.AddTag("D");
		tags.AddTag("E");
		tags.AddTag("F");
		tags.AddTag("G");
		tags.AddTag("H");
		tags.AddTag("I");
		tags.AddTag("J");
		tags.AddTag("K");
		tags.AddTag("L");
		tags.AddTag("LL");
		tags.AddTag("M");
		tags.AddTag("N");
		tags.AddTag("O");
		tags.AddTag("P");
		tags.AddTag("Q");
		tags.AddTag("R");
		tags.AddTag("S");
		tags.AddTag("T");
		tags.AddTag("U");
		tags.AddTag("V");
		tags.AddTag("W");
		tags.AddTag("X");
		tags.AddTag("Y");
		tags.AddTag("Z");
		tags.AddTag("0");
		tags.AddTag("1");
		tags.AddTag("2");
		tags.AddTag("3");
		tags.AddTag("4");
		tags.AddTag("5");
		tags.AddTag("6");
		tags.AddTag("7");
		tags.AddTag("8");
		tags.AddTag("9");
		tags.AddTag("10");
	}

	FilterTagHandler::Result t1, t2, t3, t4;

	tags.SetTag(&t1, "A"); tags.SetTag(&t1, "7"); tags.SetTag(&t1, "MIRY");
	tags.SetTag(&t2, "A"); tags.SetTag(&t2, "7"); tags.SetTag(&t2, "MIRY");
	tags.SetTag(&t3, "A"); tags.SetTag(&t3, "8"); tags.SetTag(&t3, "MIRY");
	tags.SetTag(&t4, "B"); tags.SetTag(&t4, "C");

	assert(t1.CompareAny(t2) == true );
	assert(t1.CompareAny(t3) == true );
	assert(t1.CompareAny(t4) != true );
	assert(t1.CompareAll(t2) == true );
	assert(t1.CompareAll(t3) != true );
	assert(t1.CompareAll(t4) != true );

	tags.Print(std::cout, &t1);
	tags.Print(std::cout, &t2);
	tags.Print(std::cout, &t3);
	tags.Print(std::cout, &t4);

	return 0;
}

