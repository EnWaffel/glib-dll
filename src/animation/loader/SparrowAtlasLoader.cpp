#include "glib/animation/loader/SparrowAtlasLoader.h"
#include "glib/utils/ImageUtils.h"
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <RapidXMLSTD.hpp>
#include <iostream>
#include "glib/math/Rect.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "glib/math/MathFunctions.h"

using namespace glib;

static std::string CleanName(const std::string& input) {
	if (input.size() < 4) return input;
	return input.substr(0, input.size() - 4);
}

struct XMLPtrs
{
	XMLFile* f;
	XMLDocument* d;
	XMLElement* e;
};

static XMLPtrs ReadXMLFile(const std::string& path)
{
	std::string error;
	XMLFile* file = OpenXMLFile(path, error);
	if (!file)
	{
		std::cout << error << std::endl;
	}

	XMLDocument* xml = CreateXMLFromFile(file, error);
	if (!xml)
	{
		std::cout << error << std::endl;
		DisposeXMLFile(file);
	}

	XMLElement* root = FirstOrDefaultElement(xml, "TextureAtlas", error);
	if (!root)
	{
		std::cout << error << std::endl;
		DisposeXMLFile(file);
		DisposeXMLObject(xml);
	}

	return { file, xml, root };
}

static float GetFloat(XMLElement* e, const std::string name)
{
	XMLAttributte* attr = e->first_attribute(name.c_str());
	if (attr)
	{
		return std::atof(attr->value());
	}
	return 0.0f;
}

static bool GetBool(XMLElement* e, const std::string name)
{
	XMLAttributte* attr = e->first_attribute(name.c_str());
	if (attr)
	{
		if (strcmp(attr->value(), "true") == 0)
		{
			return true;
		}
	}
	return false;
}


static bool HasAttribute(XMLElement* e, const std::string name)
{
	XMLAttributte* attr = e->first_attribute(name.c_str());
	if (attr)
	{
		return true;
	}
	return false;
}

static void flip_image_horizontally(unsigned char* data, int width, int height, int channels) {
	int row_size = width * channels;
	std::vector<unsigned char> row(row_size);

	for (int y = 0; y < height; ++y) {
		unsigned char* row_ptr = data + y * row_size;
		std::copy(row_ptr, row_ptr + row_size, row.begin());
		for (int x = 0; x < width; ++x) {
			for (int c = 0; c < channels; ++c) {
				row_ptr[(width - 1 - x) * channels + c] = row[x * channels + c];
			}
		}
	}
}

unsigned char* rotate_image(const unsigned char* input_image, int input_width, int input_height, int channels, float angle, int* output_width, int* output_height) {
	// Convert angle to radians
	float radians = angle * M_PI / 180.0;

	// Calculate the dimensions of the output image
	*output_width = (int)(fabs(input_width * cos(radians)) + fabs(input_height * sin(radians)));
	*output_height = (int)(fabs(input_height * cos(radians)) + fabs(input_width * sin(radians)));

	// Allocate memory for the rotated image
	unsigned char* output_image = (unsigned char*)malloc((*output_width) * (*output_height) * channels);
	if (!output_image) {
		printf("Failed to allocate memory for rotated image\n");
		return NULL;
	}

	// Initialize output image to black or any other default color
	for (int i = 0; i < (*output_width) * (*output_height) * channels; ++i) {
		output_image[i] = 0; // Black
	}

	// Calculate the center of the input and output images
	int cx_in = input_width / 2;
	int cy_in = input_height / 2;
	int cx_out = *output_width / 2;
	int cy_out = *output_height / 2;

	// Perform the rotation
	for (int y = 0; y < *output_height; ++y) {
		for (int x = 0; x < *output_width; ++x) {
			// Calculate the coordinates in the original image
			int src_x = (int)((x - cx_out) * cos(radians) + (y - cy_out) * sin(radians) + cx_in);
			int src_y = (int)(-(x - cx_out) * sin(radians) + (y - cy_out) * cos(radians) + cy_in);

			// Check if the source coordinates are within bounds
			if (src_x >= 0 && src_x < input_width && src_y >= 0 && src_y < input_height) {
				for (int c = 0; c < channels; ++c) {
					output_image[(y * (*output_width) + x) * channels + c] = input_image[(src_y * input_width + src_x) * channels + c];
				}
			}
		}
	}

	return output_image;
}


std::map<std::string, Animation*>glib::SparrowAtlasLoader::LoadFile(const std::string& path, const std::string& imagePath, Window* wnd, bool pixelart, bool xFlipped)
{
	return LoadFile(path, imagePath, wnd, pixelart, xFlipped, {});
}

AnimationFrame GetFrame(XMLElement* e, Texture* tex)
{
	AnimationFrame frame{};

	bool trimmed = HasAttribute(e, "frameX");
	bool rotated = (HasAttribute(e, "rotated") && GetBool(e, "rotated"));
	bool flipX = (HasAttribute(e, "flipX") && GetBool(e, "flipX"));
	bool flipY = (HasAttribute(e, "flipY") && GetBool(e, "flipY"));

	Rect rect(GetFloat(e, "x"), GetFloat(e, "y"), GetFloat(e, "width"), GetFloat(e, "height"));

	Rect size;
	if (trimmed)
	{
		size = Rect(GetFloat(e, "frameX"), GetFloat(e, "frameY"), GetFloat(e, "frameWidth"), GetFloat(e, "frameHeight"));
	}
	else
	{
		size = Rect(0.0f, 0.0f, rect.w, rect.h);
	}

	Vec2 sourceSize = Vec2(size.w, size.h);
	if (rotated && !trimmed) sourceSize = Vec2(size.h, size.w);

	frame.uvPos = Vec2(rect.x / tex->width, rect.y / tex->height);
	frame.uvSize = Vec2(rect.w / tex->width, rect.h / tex->height);
	frame.size = sourceSize;
	frame.rotation = rotated ? -90.0f : 0.0f;
	frame.offset = glib::Vec2(-size.x, -size.y);

	return frame;
}

std::map<std::string, Animation*> glib::SparrowAtlasLoader::LoadFile(const std::string& path, const std::string& imagePath, Window* wnd, bool pixelart, bool xFlipped, const std::map<std::string, int>& fpsMap)
{
	std::map<std::string, Animation*> anims;
	XMLPtrs ptrs = ReadXMLFile(path);
	XMLElement* root = ptrs.e;

	std::vector<std::string> names;
	std::map<std::string, std::vector<AnimationFrame>> frames;

	ImageData imgData = wnd->LoadTextureRaw(imagePath);
	Texture* tex = wnd->LoadTextureFromRawData(imgData, pixelart);
	int* refCount = new int;
	*refCount = 1;

	bool first = true;
	for (XMLElement* e = root->first_node("SubTexture"); e; e = e->next_sibling())
	{
		std::string name = CleanName((e->first_attribute("name")->value()));

		if (frames.count(name) < 1)
		{
			frames.insert({ name, std::vector<AnimationFrame>() });
		}
		frames.at(name).push_back(GetFrame(e, tex));
	}

	for (const auto& v : frames)
	{
		int fps = 24;
		if (fpsMap.count(v.first) > 0) fps = fpsMap.at(v.first);
		anims.insert({ v.first, new Animation(v.second, tex, fps, false, refCount) });
	}

	free(imgData.data);

	DisposeXMLFile(ptrs.f);
	DisposeXMLObject(ptrs.d);

	if (anims.empty())
	{
		delete tex;
		delete refCount;
	}
	return anims;
}
