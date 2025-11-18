#pragma once
#include <string>
#include <fstream>
#include <ranges>
#include <cctype>
#include <iostream>

#include "Models/Mesh.h"
#include "Scene/Objects/Object.h"

Vec3<float> extractVertexVec3(std::vector<std::string>& tokens) {
	float x;
	float y;
	float z;

	auto result = std::from_chars(tokens[1].data(), tokens[1].data() + tokens[1].size(), x);
	if (result.ec != std::errc()) {
		throw std::runtime_error("Failed to parse float from 2nd word in string: " + tokens[1]);
	}

	result = std::from_chars(tokens[2].data(), tokens[2].data() + tokens[2].size(), y);
	if (result.ec != std::errc()) {
		throw std::runtime_error("Failed to parse float from 3rd word in string: " + tokens[2]);
	}
	
	result = std::from_chars(tokens[3].data(), tokens[3].data() + tokens[3].size(), z);
	if (result.ec != std::errc()) {
		throw std::runtime_error("Failed to parse float from 4th word in string: " + tokens[3]);
	}

	return Vec3<float>(x, y, z);
}

Vec2<float> extractTextureCoords(std::vector<std::string>& tokens) {
	float x;
	float y;

	auto result = std::from_chars(tokens[1].data(), tokens[1].data() + tokens[1].size(), x);
	if (result.ec != std::errc()) {
		throw std::runtime_error("Failed to parse float from 2nd word in string: " + tokens[1]);
	}

	result = std::from_chars(tokens[2].data(), tokens[2].data() + tokens[2].size(), y);
	if (result.ec != std::errc()) {
		throw std::runtime_error("Failed to parse float from 3rd word in string: " + tokens[2]);
	}

	return Vec2<float>(x, y);
}

std::vector<std::string> extractWords(std::string input, char delimiter) {
	std::vector<std::string> tokens;

	for (const auto token : std::views::split(input, delimiter)) {
		std::string word = std::ranges::to<std::string>(token);
		tokens.push_back(word);
	}

	return tokens;
}

std::vector<VertexIn> extractFaceVertices(std::vector<std::string>& tokens, std::vector<Vec3<float>>& verticesPos, std::vector<Vec2<float>>& textureCoords, std::vector<Vec3<float>>& normals) {
	/*
	// handle the up to 3 attributes divided in slashes with however many vertices that will be used.
	// there can be more than 3 vertices specified, so if that's the case we should fan/triangulate our faces
	// given f: 1 2 3 4 5 ->  1 2 3, 1 3 4, 1 4 5
	*/
	// ex: f 302/344/1791 750/792/1780 315/357/743
	//
	
	// tokens[0] = the identifier.
	// tokens[1] is the start
	std::vector<VertexIn> vertices;

	std::vector<std::string> firstVert = extractWords(tokens[1], '/'); //tokens[1];
	std::vector<std::string> secondVert = extractWords(tokens[2], '/'); //= tokens[2];

	int numAttributes = static_cast<int>(firstVert.size()); // if 1 only v is defined, if 2 only v, & vt defined, if 3 either v, vt & vn or only v & vn.
	VertexIn v1;
	VertexIn v2;
	VertexIn v3;

	for (int i = 3; i < tokens.size(); i++) {
		//std::string secondVert = tokens[i - 1]; // move this out for initial, then swap what was collected in third vertex into the second vert, so we don't do duplicate processing.
		std::vector<std::string> thirdVert = extractWords(tokens[i], '/');

		switch (numAttributes) {
		case 1:
			v1 = VertexIn(verticesPos[std::stoi(firstVert[0]) - 1]);
			v2 = VertexIn(verticesPos[std::stoi(secondVert[0]) - 1]);
			v3 = VertexIn(verticesPos[std::stoi(thirdVert[0]) - 1]);
			break;
		case 2:
			v1 = VertexIn(verticesPos[std::stoi(firstVert[0]) - 1], {}, textureCoords[std::stoi(firstVert[1]) - 1]);
			v2 = VertexIn(verticesPos[std::stoi(secondVert[0]) - 1], {}, textureCoords[std::stoi(secondVert[1]) - 1]);
			v3 = VertexIn(verticesPos[std::stoi(thirdVert[0]) - 1], {}, textureCoords[std::stoi(thirdVert[1]) - 1]);
			break;
		case 3:
			// this means no texture coord is included, only normals.
			if (firstVert[1] == "") {
				v1 = VertexIn(verticesPos[std::stoi(firstVert[0]) - 1], normals[std::stoi(firstVert[2]) - 1]);
				v2 = VertexIn(verticesPos[std::stoi(secondVert[0]) - 1], normals[std::stoi(secondVert[2]) - 1]);
				v3 = VertexIn(verticesPos[std::stoi(thirdVert[0]) - 1], normals[std::stoi(thirdVert[2]) - 1]);
			}
			else {
				v1 = VertexIn(verticesPos[std::stoi(firstVert[0]) - 1], normals[std::stoi(firstVert[2]) - 1], textureCoords[std::stoi(firstVert[1]) - 1]);
				v2 = VertexIn(verticesPos[std::stoi(secondVert[0]) - 1], normals[std::stoi(secondVert[2]) - 1], textureCoords[std::stoi(secondVert[1]) - 1]);
				v3 = VertexIn(verticesPos[std::stoi(thirdVert[0]) - 1], normals[std::stoi(thirdVert[2]) - 1], textureCoords[std::stoi(thirdVert[1]) - 1]);
			}

			break;
		default:
			throw std::runtime_error("Object being loaded has too many attributes for the face values. String: " + tokens[1]);
			//return std::vector<VertexIn>();
		}

		vertices.push_back(v1);
		vertices.push_back(v3);
		vertices.push_back(v2);
		
		// keep progressing 
		secondVert = thirdVert;
	}

	// need to also think about creating tangent and bitangent for normal mapping in here
	// maybe a normal of 0, 0, 0, signals to our engine that we need to fallback to flat shading instead.

	return vertices;
}

// need to implement our own trimming solution
std::string trim(std::string inputString) {
	auto isSpace = [](unsigned char c) { return std::isspace(c); };

	std::string trimmedString = std::ranges::to<std::string>(inputString | std::views::drop_while(isSpace) | std::views::reverse |
		std::views::drop_while(isSpace) | std::views::reverse);

	return trimmedString;
}

enum class OBJSpecifiers {
	V
};

Object loadOBJ(const std::string& path, float size=1.f) {
	

	std::ifstream file{ path };

	if (!file) {
		throw std::runtime_error("Couldn't open OBJ file at path: " + path + "\n");
	}

	std::string line;
	std::vector<Vec3<float>> objVertices; // make
	//Vec3<float> vertexIds; // can turn this into a range, as my vertices will be duplicated, so it will be T1: 0, 1, 2. T2: 3, 4, 5 .... etc
	std::vector<Vec2<float>> objTextureCoords;
	std::vector<Vec3<float>> objNormalCoords;

	std::vector<VertexIn> objMeshVertices;
	// maybe make a v,  vn, vt map and have it so that we use that to specifically add/grab from based on the / ?
	// v/vt/vn is the order for the face

	while (std::getline(file, line)) {
		line = trim(line);
		//std::string_view line = lineBuffer;
		if (line.empty()) {
			continue;
		}

		std::vector<std::string> tokens = extractWords(line, ' ');
		std::string prefix = tokens[0];

		if (prefix == "v") {
			if (tokens.size() < 4) {
				continue;
			}

			objVertices.push_back(extractVertexVec3(tokens));
		}
		else if (prefix == "vn") {
			if (tokens.size() < 4) {
				continue;
			}

			objNormalCoords.push_back(extractVertexVec3(tokens));
		}
		else if (prefix == "vt") {
			if (tokens.size() < 3) {
				continue;
			}

			objTextureCoords.push_back(extractTextureCoords(tokens));
		}
		else if (prefix == "f") {
			if (tokens.size() < 4) {
				continue;
			}

			// maybe instead of extraction sending back something I could refactor to pass in a reference of my objMeshVertices and just pushback in there ! TODO!!!!!!!!!!
			std::vector<VertexIn> faceVerts = extractFaceVertices(tokens, objVertices, objTextureCoords, objNormalCoords);
			objMeshVertices.insert(objMeshVertices.end(), faceVerts.begin(), faceVerts.end());
		}
		else if (prefix == "g") {
			// this should handle grouping. Will worry about this in a later refactor TODO
			// this should allow us to handle scenes with multiple objects, should generate each object that is separate from the others.
			continue;
		}
		else {
			continue;
		}
	}

	std::vector<uint32_t> vertexIds = std::ranges::to<std::vector>(std::ranges::iota_view(0u, objMeshVertices.size()));

	Object obj = Object(size, objMeshVertices, vertexIds);

	return obj;
}
