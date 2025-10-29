
#include "shape.h"
#include "endianess.h"
#include <fstream>

static std::string GetFileName(const char *name, uint64_t key)
{
	std::string used = "saves/shapes/";
	if (strlen(name) == 0)
	{
		used += "shape";
		used += std::to_string(key);
		used += ".shape";
		return (used);
	}
	used += name;
	used += ".shape";
	return (used);
}

// Shape data format in the binary file:
// [key][vertex size][vertex data][index size][index data][bounding box]

void SaveNewShape(uint64_t shapeKey, const char *name = "")
{
	std::string fileName = GetFileName(name, shapeKey);
	std::ofstream outFile(fileName, std::ios::binary);
	if (!outFile)
		return ;

	t_DataForShape &data = GetShapeDataWithKey(shapeKey);
	uint32_t vertSize = (uint32_t)data.vertexData.size();
	uint32_t indSize = (uint32_t)data.indexData.size();

	uint64_t leShapeKey = ToLittleEndian(shapeKey);
	uint32_t leVertSize = ToLittleEndian(vertSize);
	uint32_t leIndSize = ToLittleEndian(indSize);

	outFile.write((const char*)&leShapeKey, sizeof(uint64_t));
	outFile.write((const char*)&leVertSize, sizeof(uint32_t));
	outFile.write((const char*)data.vertexData.data(), data.vertexData.size() * sizeof(Vertex));
	outFile.write((const char*)&leIndSize, sizeof(uint32_t));
	outFile.write((const char*)data.indexData.data(), data.indexData.size() * sizeof(GLuint));
	outFile.write((const char*)&data.bBox, sizeof(t_BoundingB));
}

uint64_t LoadShape(const char *name)
{
	t_DataForShape data;
	uint64_t shapeKey = 0;
	uint32_t vertSize = 0;
	uint32_t indSize = 0;
	std::ifstream inFile(name, std::ios::binary);
	if (!inFile)
		return (1);
	inFile.read((char*)&shapeKey, sizeof(uint64_t));
	shapeKey = FromLittleEndian(shapeKey);

	inFile.read((char*)&vertSize, sizeof(uint32_t));
	vertSize = FromLittleEndian(vertSize);
	size_t vertCount = vertSize;
	data.vertexData = std::vector<Vertex>(vertCount);
	inFile.read((char*)data.vertexData.data(), data.vertexData.size() * sizeof(Vertex));

	inFile.read((char*)&indSize, sizeof(uint32_t));
	indSize = FromLittleEndian(indSize);
	size_t indCount = indSize;
	data.indexData = std::vector<GLuint>(indCount);
	inFile.read((char*)data.indexData.data(), data.indexData.size() * sizeof(GLuint));

	inFile.read((char*)&data.bBox, sizeof(t_BoundingB));

	AddShapeToHolder(data, shapeKey);
	return (shapeKey);
}
