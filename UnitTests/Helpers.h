#pragma once
#include "stdafx.h"
#include <cstdio>
#include <iosfwd>
#include <array>

namespace Helpers
{
	inline std::array< std::array<double, 5>, 50> GenerateCsvFile(const char* fileName)
	{
		const auto linesNr = 50;
		std::stringstream ss;
		const auto headerLine = "class, a, b, c, d";
		ss << headerLine << "\n";
		std::array<std::array<double, 5>, 50> lines{};
		srand(1);
		for (auto i = 0; i < linesNr; ++i)
		{
			lines[i][0] = rand() % 3;
			ss << lines[i][0] << ",";
			for (auto j = 1; j < 5; ++j)
			{
				lines[i][j] = rand()*0.001;
				ss << lines[i][j] << ",";
			}
			ss.seekp(-1, std::stringstream::cur);
			ss << "\n";
		}

		FILE* fp;
		fopen_s(&fp, fileName, "w+");
		if (fp == nullptr)
			return lines;
		fprintf(fp, "%s", ss.str().c_str());
		fclose(fp);
		return lines;

	};
}
