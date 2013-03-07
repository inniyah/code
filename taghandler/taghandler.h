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

#ifndef _TAGHANDLER_H
#define _TAGHANDLER_H

#include <string>
#include <vector>
#include <iostream>

class FilterTagHandler
{
protected:
	class Element
	{
	public:
		inline Element(const std::string tag_name, unsigned int tag_position, unsigned int tag_flag)
		{ name = tag_name; position = tag_position; flag = tag_flag; }

		std::string name;
		unsigned int position;
		unsigned int flag;
	};

public:
	class Result
	{
	public:
		inline Result()
		{ size = 1; data = new unsigned int[size]; for (unsigned int i=0; i<size; i++) data[i] = 0; }

		~Result()
		{ if (data) delete[] data; }

		inline void Clear()
		{
			for (unsigned int i=0; i<size; i++) data[i] = 0;
		}

		inline void SetTag(unsigned int position, unsigned int flag)
		{
			Size(position+1);
			data[position] = data[position] | flag;
		}

		inline void ResetTag(unsigned int position, unsigned int flag)
		{
			Size(position+1);
			data[position] = data[position] & ~flag;
		}

		inline bool CheckAny(unsigned int position, unsigned int flag)
		{
			Size(position+1);
			if ((data[position] & flag) != 0) return true;
			return false;
		}

		inline bool CheckAll(unsigned int position, unsigned int flag)
		{
			Size(position+1);
			if ((data[position] & flag) == flag) return true;
			return false;
		}

		bool CompareAny(const Result &red) const;
		bool CompareAll(const Result &green) const;
		void Print(std::ostream &out) const;

	protected:
		void Size(unsigned int new_size)
		{
			if (new_size > size)
			{
				unsigned int *new_data = new unsigned int[new_size];
				for (unsigned int i=0; i<new_size; i++) new_data[i] = 0;
				if (data)
				{
					for (unsigned int i=0; i<size; i++) new_data[i] = data[i];
					delete[] data;
				}
				data = new_data;
				size = new_size;
			}
		}

		unsigned int size;
		unsigned int *data;
	};

public:
	FilterTagHandler();
	~FilterTagHandler();

	const Element *GetTag(const std::string name);
	void AddTag(const std::string name);
	bool TagExists(const std::string name);
	void PrintAll(std::ostream &out) const;
	void Print(std::ostream &out, Result *result) const;

	inline void Clear()
	{ elements.clear(); current_position=0; current_flag=1; };

	inline void SetTag(Result *result, const std::string name)
	{
		AddTag(name);
		const Element *e = GetTag(name);
		result->SetTag(e->position, e->flag);
	}

	inline void SetTagIfExists(Result *result, const std::string name)
	{
		const Element *e = GetTag(name);
		if (e) result->SetTag(e->position, e->flag);
	}

	inline void ResetTag(Result *result, const std::string name)
	{
		AddTag(name);
		const Element *e = GetTag(name);
		result->ResetTag(e->position, e->flag);
	}

	inline void ResetTagIfExists(Result *result, const std::string name)
	{
		const Element *e = GetTag(name);
		if (e) result->ResetTag(e->position, e->flag);
	}

	inline bool CheckTag(Result *result, const std::string name)
	{
		//AddTag(name);
		const Element *e = GetTag(name);
		return e ? result->CheckAny(e->position, e->flag) : false;
	}

protected:
	std::vector<Element> elements;

	unsigned int current_position;
	unsigned int current_flag;
};

#endif // _TAGHANDLER_H

