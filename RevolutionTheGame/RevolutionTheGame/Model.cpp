#include "Model.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <array>

Model::Model() :
	m_texture((Texture2D*)NULL)
{
	//LoadVerticesFromFile("cube.txt");
	InitializeBuffers();
}

Model::Model(const VerticesType& ver) :
	vertices(ver),
	m_texture((Texture2D*)NULL)
{
	InitializeBuffers();
}

Model::Model(const VerticesType& ver, Texture *_tex) :
	vertices(ver),
	m_texture(_tex)
{
	InitializeBuffers();
}

Model::Model(const char *_verFilename, Texture *_tex) :
	m_texture(_tex)
{
	LoadVerticesFromFile(_verFilename);
	InitializeBuffers();
}

Model::Model(const char *_verFilename, Texture *_tex, Texture2D *_normalMap) :
	m_texture(_tex),
	m_normalMap(_normalMap)
{
	LoadVerticesFromFile(_verFilename);
	InitializeBuffers();
}

Model::~Model()
{
	ShutdownBuffers();
}

void Model::ComputeTangentsBitangents()
{
	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		GMath::Vertex& v0 = vertices[i];
		GMath::Vertex& v1 = vertices[i + 1];
		GMath::Vertex& v2 = vertices[i + 2];

		GMath::Vector3f deltaPos1 = v1.pos - v0.pos;
		GMath::Vector3f deltaPos2 = v2.pos - v0.pos;

		GMath::Vector2f deltaUV1 = v1.tex - v0.tex;
		GMath::Vector2f deltaUV2 = v2.tex - v0.tex;

		float r = 1.0f / ((deltaUV1.tu * deltaUV2.tv) - (deltaUV1.tv * deltaUV2.tu));
		GMath::Vector3f tangent = ((deltaPos1 * deltaUV2.tv) - (deltaPos2 * deltaUV1.tv)) * r;
		GMath::Vector3f bitangent = ((deltaPos2 * deltaUV1.tu) - (deltaPos1 * deltaUV2.tu)) * r;

		v0.tangent = tangent;
		v1.tangent = tangent;
		v2.tangent = tangent;

		v0.bitangent = bitangent;
		v1.bitangent = bitangent;
		v2.bitangent = bitangent;
	}

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		//t = glm::normalize(t - n * glm::dot(n, t));
		GMath::Vertex& v = vertices[i];
		v.tangent = GMath::Normalize(
			v.tangent - (v.nor * GMath::Dot(v.nor, v.tangent)));
		
		if (GMath::Dot(GMath::Cross(v.nor, v.tangent), v.bitangent) < 0.0f)
		{
			v.tangent = v.tangent * -1.0f;
		}
	}
}

void Model::InitializeBuffers()
{
    m_meshSize = vertices.size();
	ComputeTangentsBitangents();
	/*vertices.clear();*/
}

void Model::ShutdownBuffers()
{

}

// Model loader
void Model::LoadVerticesFromFile(const char *_filename)
{
    std::ifstream inStream(_filename, std::ios::in);

    if (!inStream.is_open())
    {
        return;
    }

    std::string sfile, line;
    std::string prefix;
    std::string first, second, third;
    std::stringstream strStream;
    std::vector < std::array<int, 3> > faces;
	std::vector<GMath::Vector3f> tmpVertices;
    std::vector<GMath::Vector3f> tmpNormal;
    std::vector<GMath::Vector2f> tmpTexture;

    while (inStream.good())
    {
        std::getline(inStream, line);

		if (line.empty() || line.front() == '#')
        {
            continue;
        }

        strStream.str(line);
        strStream >> prefix;

        if (prefix == "vt") // texture
        {
            float s, t;
            strStream >> s >> t;
			tmpTexture.push_back(GMath::Vector2f(s, t));
        }
        else if (prefix == "vn") // normal
        {
            float x, y, z;
            strStream >> x >> y >> z;
            tmpNormal.push_back(GMath::Vector3f(x, y, z));
        }
        else if (prefix == "v") // vertex
        {
            float x, y, z;
            strStream >> x >> y >> z;
            tmpVertices.push_back(GMath::Vector3f(x, y, z));
        }
        else if (prefix == "f") // face
        {
            strStream >> first;
            strStream >> second;
            strStream >> third;

            int fv, ft, fn;
            int sv, st, sn;
            int tv, tt, tn;
            char t;

            strStream.clear();
            strStream.str(first);
            strStream >> fv >> t >> ft >> t >> fn;
            strStream.clear();
            strStream.str(second);
            strStream >> sv >> t >> st >> t >> sn;
            strStream.clear();
            strStream.str(third);
            strStream >> tv >> t >> tt >> t >> tn;

            std::array<int, 3> fa = {fv - 1, fn - 1, ft - 1};
            std::array<int, 3> sa = {sv - 1, sn - 1, st - 1};
            std::array<int, 3> ta = {tv - 1, tn - 1, tt - 1};
            faces.push_back(fa);
            faces.push_back(sa);
            faces.push_back(ta);
        }

        strStream.clear();
    }

    inStream.close();

	if (tmpNormal.size() > 0)
	{
		for (auto face : faces)
		{
			vertices.push_back(GMath::Vertex(
				tmpVertices[face[0]], tmpNormal[face[1]], tmpTexture[face[2]]
			));
		}
	}
	else
	{
		for (auto face : faces)
		{
			vertices.push_back(GMath::Vertex(
				tmpVertices[face[0]], GMath::Vector3f(0.0f, 0.0f, 0.0f), tmpTexture[face[2]]
			));
		}
	}

    m_meshSize = vertices.size();
}
