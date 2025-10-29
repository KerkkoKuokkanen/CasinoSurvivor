
#include "shape.h"
#include "tesselator.h"
#include <unordered_map>
#include <random>

#define BIG_VALUE 9999999.9f
#define SMALL_VALUE -9999999.9f

t_DataForShape empty = {};

static void* libtessAlloc(void* userData, unsigned int size)
{
	void *ret = malloc(size);
	return (ret);
}

static void* libtessRealloc(void* userData, void* ptr, unsigned int size)
{
	void *ret = realloc(ptr, size);
	if (ptr != NULL)
		free(ptr);
	return (ret);
}

static void libtessFree(void* userData, void* ptr)
{
	free(ptr);
}

static std::unordered_map<uint64_t, t_DataForShape> &GetShapeRegistry()
{
	static std::unordered_map<uint64_t, t_DataForShape> shapeRegistry;
	return (shapeRegistry);
}

static uint64_t GenerateRandomNumber()
{
	static std::mt19937_64 gen(std::random_device{}());
	static std::uniform_int_distribution<uint64_t> distrib(0, (1ULL << 42) - 1);
	return distrib(gen);
}

static uint64_t UniqueShapeKeyGenerator()
{
	static uint64_t key = rand() % 4194304;
	uint64_t randomPart = GenerateRandomNumber();
	key += 1;
	if (key >= 4194304)
		key = 0;
	uint64_t ret = (key << 42) | randomPart;
	return (ret);
}

t_DataForShape &GetShapeDataWithKey(uint64_t key)
{
	auto &registry = GetShapeRegistry();
	auto it = registry.find(key);
	if (it != registry.end())
		return (it->second);
	return (empty);
}

void AddShapeToHolder(t_DataForShape data, uint64_t key)
{
	auto &registry = GetShapeRegistry();
	registry[key] = data;
}

uint64_t CreateGLShapeData(std::vector<float> &points)
{
	t_DataForShape shapeData;

	TESSalloc allocator = {0};
	allocator.memalloc = libtessAlloc;
	allocator.memrealloc = libtessRealloc;
	allocator.memfree = libtessFree;
	TESStesselator* tess = tessNewTess(&allocator);

	const unsigned int size = (int)points.size();
	float *vertecies = (float*)malloc(sizeof(float) * size);
	for (int i = 0; i < size; i++)
		vertecies[i] = points[i];
	tessAddContour(tess, 2, vertecies, sizeof(float) * 2, size / 2);
	int tester = tessTesselate(tess, TESS_WINDING_NONZERO, TESS_POLYGONS, 3, 2, NULL);
	if (tester == 0)
		printf("tessalation fail\n");

	int numVertices = tessGetVertexCount(tess);
	int numIndecies = tessGetElementCount(tess);

	const float* tessVertices = tessGetVertices(tess);
	const int* tessIndices = tessGetElements(tess);

	shapeData.indexData = std::vector<GLuint>(tessIndices, tessIndices + (numIndecies * 3));
	int count = 0;
	float xMin = BIG_VALUE, yMin = BIG_VALUE;
	float xMax = SMALL_VALUE, yMax = SMALL_VALUE;
	for (int i = 0; i < numVertices; i++)
	{
		float x = tessVertices[count];
		float y = tessVertices[count + 1];
		Vertex vert = {glm::vec3(x, y, 0.0f), glm::vec2(x, -y)};
		xMin = (x < xMin) ? x : xMin;
		yMin = (y < yMin) ? y : yMin;
		xMax = (x > xMax) ? x : xMax;
		yMax = (y > yMax) ? y : yMax;
		shapeData.vertexData.push_back(vert);
		count += 2;
	}
	free(vertecies);
	tessDeleteTess(tess);
	t_BoundingB box = {{xMin, yMax}, {xMax, yMax}, {xMax, yMin}, {xMin, yMin}};
	shapeData.bBox = box;
	auto &registry = GetShapeRegistry();
	uint64_t useKey = UniqueShapeKeyGenerator();
	registry[useKey] = shapeData;
	return (useKey);
}
