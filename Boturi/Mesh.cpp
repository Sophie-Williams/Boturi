#include "Boturi.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>
void Mesh::loadModel(const char * filename) 
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename))
		throw std::runtime_error(err);

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) 
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

Mesh::Mesh() { }

Mesh::Mesh(const char * filename)
{
	loadModel(filename);

	indexCount = indices.size();

	VkDeviceSize vertListSize = sizeof(vertices[0]) * vertices.size();
	VkDeviceSize indListSize = sizeof(indices[0]) * indices.size();

	vertexBuffer = Buffer(vertListSize, 
						  vertices.data(), 
						  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

	indexBuffer = Buffer(indListSize,
						 indices.data(),
						 VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
}

void Mesh::cleanup()
{
	vertexBuffer.cleanup();
	indexBuffer.cleanup();
}

VkBuffer Mesh::getVertexBuffer() { return vertexBuffer.getBuffer(); }
VkBuffer Mesh::getIndexBuffer() { return indexBuffer.getBuffer(); }
uint32_t Mesh::getIndexCount() { return indexCount; }