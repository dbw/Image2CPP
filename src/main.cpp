#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include "string"
#include <fstream>
#include <iostream>
#include <iomanip>

std::string remove_extension(const std::string& path) {
	if (path == "." || path == "..")
		return path;

	size_t pos = path.find_last_of("\\/.");
	if (pos != std::string::npos && path[pos] == '.')
		return path.substr(0, pos);

	return path;
}
int main(int argc, char* argv[])
{
	if (argc == 1)
		return 0;
	int w, h, n;

	for (int arg = 1; arg < argc; ++arg)
	{
		unsigned char* data = stbi_load(argv[arg], &w, &h, &n, 0);
		std::string outName = remove_extension(argv[arg]);

		auto start = outName.find_last_of("\\/");
		if (start != outName.npos)
			start++;
		auto imageName = outName.substr(start);
		
		std::cout << "Image: " << imageName << " - ";
		{
			auto headerName = outName + ".h";
			std::ofstream header(headerName);
			std::cout << "Writing to " << headerName << ", ";

			for (auto& c : imageName) c = toupper(c);
			header << "const unsigned int " << imageName << "_WIDTH = " << w << ";\n";
			header << "const unsigned int " << imageName << "_HEIGHT = " << h << ";\n";
			header << "const unsigned int " << imageName << "_DEPTH = " << n << ";\n";
			for (auto& c : imageName) c = tolower(c);
			header << "extern char " << imageName << "_data[];";
		}

		outName +=".cpp";
		std::ofstream out(outName);
		std::cout << outName << "\n";

		out << std::showbase << std::internal << std::setfill('0') << std::hex;		
		out << "char " << imageName << "_data[] = {\n";	

		auto ptr = data;

		for (auto x = 0; x < h; ++x)
		{		
			for (auto y = 0; y < w; ++y)
			{
				out << "\t";
				for (auto c = 0; c < n; ++c)
				{			
					out << "\'" << std::setw(4) << static_cast<unsigned int>(*ptr++) << "\',";
				}
				out << "\n";
			}
			std::cout << x << "/" << h << "\r";
		}
		out << "};\n";
		stbi_image_free(data);
	}
	return 0;
}
