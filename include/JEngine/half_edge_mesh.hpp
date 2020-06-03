#pragma once
#include <macros.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <vertex.hpp>
#include <half_edge.hpp>
#include <half_edge_vertex.hpp>
#include <face.hpp>

jeBegin

class HalfEdgeMesh
{
public:

	using HalfEdgeMap = std::unordered_map<std::string, HalfEdge*>;
	using HalfEdges = std::vector<HalfEdge*>;
	using HalfEdgePair = std::pair<std::string, HalfEdge*>;
	using Faces = std::vector<Face>;
	using ConstFaces = std::vector<const Face*>;

	HalfEdgeMesh(std::vector<Vertex> vertices, 
		std::vector<unsigned> indices)	
	{ 
		set(vertices, indices); 
	}

	~HalfEdgeMesh(void)
	{
		// delete edges
		for (auto& edge_pair : hEdges_)
			delete edge_pair.second;
	}

	std::string get_edge_key(unsigned from, unsigned to)
	{
		return std::to_string(from) + " " + std::to_string(to);
	}

	ConstFaces get_neighbor_faces(unsigned vert_index) const
	{
		ConstFaces neighbors;

		const HalfEdgeVertex* vert = &vertices_[vert_index];
		const HalfEdge* cur_edge = vert->edge;
		const HalfEdge* end = cur_edge;

		bool backwards = false;

		// skip if vertex is unused
		if (cur_edge == nullptr)
			return neighbors;

		do
		{
			// add current vertex
			if (cur_edge->face)
				neighbors.push_back(cur_edge->face);

			HalfEdge* next;

			// find next half edge
			if (backwards)
			{
				HalfEdge* prev = cur_edge->Prev();
				if (prev)
					next = prev->twin;
				else
					next = nullptr;
			}
			else
				next = cur_edge->twin->next;

			if (next == nullptr)
			{
				if (backwards)
					break;

				backwards = true;
				std::swap(end, cur_edge);
				continue;
			}
			else
				cur_edge = next;

		} while (cur_edge != end);

		return neighbors;
	}

	const Faces& get_faces() const { return faces_; }

private:

	using Vertices = std::vector<HalfEdgeVertex>;

	HalfEdgeMap hEdges_;
	HalfEdges boundary_;
	Faces faces_;
	Vertices vertices_;

	void set(std::vector<Vertex> vertices, std::vector<unsigned> indices)
	{
		// reserve space for vertices
		vertices_.reserve(vertices.size());

		// copy vertex positions
		for (const Vertex& vert : vertices)
			vertices_.push_back(HalfEdgeVertex(vert.position));

		// reserve space for faces
		unsigned num_indices = unsigned(indices.size());
		faces_.reserve(num_indices / 3);

		// create faces
		for (unsigned i = 0; i < num_indices; i += 3)
		{
			// create face
			auto face_it = faces_.insert(faces_.end(), Face());
			Face* cur_face = &(*face_it);

			// create half edges
			HalfEdge* prev = nullptr;
			for (unsigned j = 0; j < 3; ++j)
			{
				// calculate vertex indices
				unsigned v1 = indices[i + j];
				unsigned v2 = indices[i + ((j + 1) % 3)];

				// check if edge exists
				std::string edge_key = get_edge_key(v1, v2);
				auto it = hEdges_.find(edge_key);

				// create half edge if needed
				if (it == hEdges_.end())
				{
					// create edge and its twin
					HalfEdge* edge = new HalfEdge();
					HalfEdge* twin = new HalfEdge();

					// set vertices
					edge->vertex = &vertices_[v2];
					twin->vertex = &vertices_[v1];

					// update vertex edges if needed
					if (edge->vertex->edge == nullptr)
						edge->vertex->edge = twin;
					if (twin->vertex->edge == nullptr)
						twin->vertex->edge = edge;

					// set face
					edge->face = cur_face;

					// set twins
					edge->twin = twin;
					twin->twin = edge;

					// insert edges
					auto inserted = hEdges_.insert(HalfEdgePair(edge_key, edge));
					it = inserted.first;
					hEdges_.insert(HalfEdgePair(get_edge_key(v2, v1), twin));

					// update face record
					if (cur_face->edge == nullptr)
						cur_face->edge = edge;
				}
				// edge exists, update internals
				else
				{
					// update face's edge
					if (cur_face->edge == nullptr)
						cur_face->edge = it->second;

					// update edge's face
					if (it->second->face == nullptr)
						it->second->face = cur_face;
				}

				// update next pointers
				if (prev != nullptr)
					prev->next = it->second;

				prev = it->second;
			}

			// find final next
			std::string next_key = get_edge_key(indices[i], indices[i + 1]);
			auto next = hEdges_.find(next_key);

			// set next
			prev->next = next->second;
		}

		// store mesh boundary
		calculate_boundary();
	}

	void calculate_boundary(void)
	{
		// find all half edges without faces
		for (auto& edge_pair : hEdges_)
		{
			HalfEdge* edge = edge_pair.second;

			if (edge->face == nullptr)
				boundary_.push_back(edge);
		}
	}

	// private copy
	HalfEdgeMesh(const HalfEdgeMesh&);

};




jeEnd