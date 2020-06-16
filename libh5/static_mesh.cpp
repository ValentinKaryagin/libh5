#include "static_mesh.h"

#include "physics_interface.h"
#include "render_interface.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(static_mesh);

	static_mesh::static_mesh()
		: object()
	{}

	static_mesh::static_mesh(const std::wstring &path)
		: object()
		, _path(path)
	{}

	static_mesh::~static_mesh()
	{
		terminate();
	}

	void static_mesh::set_path(const std::wstring &path)
	{
		_path = path;
	}

	int32_t static_mesh::init()
	{
		int32_t error_level = 0;

		const size_t path_length = _path.length();
		size_t ext_length = 0;		

		while (_path[path_length - ++ext_length] != u('.'));

		const std::wstring ext = _path.substr(path_length - ext_length);

		if (ext == u(".stm"))
		{
			error_level = read_native();
		}
		else if (ext == u(".obj"))
		{
			error_level = read_wavefront();
		}		

		if (!error_level)
		{
			error_level = physics_interface::get_instance()->init_context(this);
		}

		if (!error_level)
		{
			error_level = render_interface::get_instance()->init_context(this);
		}

		return error_level;
	}

	void static_mesh::terminate()
	{
		refs = 0;

		render_interface::get_instance()->terminate_context(this);
		physics_interface::get_instance()->terminate_context(this);

		indices.clear();		
		vertices.clear();
	}

	int32_t static_mesh::read_native()
	{
		int32_t error_level = 0;

		std::ifstream file(_path, std::ios::in | std::ios::binary);
		if (!file)
		{
			return error_level;
		}

		if (!error_level)
		{
			char header[6]{};

			file.read(header, 6);

			if (strcmp(header, "h5stm") != 0)
			{
				return error_level;
			}

			if (!error_level)
			{
				size_t vertices_count = 0;
				file.read((char *)&vertices_count, sizeof(size_t));
				if (!vertices_count)
				{
					return error_level;
				}

				for (size_t i = 0; i < vertices_count; ++i)
				{
					vertex v;
					file.read((char *)&v, sizeof(vertex));
					vertices.push_back(v);
				}

				size_t indices_count = 0;
				file.read((char *)&indices_count, sizeof(size_t));
				if (!indices_count)
				{
					return error_level;
				}

				for (size_t i = 0; i < indices_count; ++i)
				{
					uint32_t index = 0;
					file.read((char *)&index, sizeof(uint32_t));
					indices.push_back(index);
				}
			}						
		}

		if (!error_level)
		{
			file.close();
		}

		return error_level;
	}

	int32_t static_mesh::read_wavefront()
	{
		int32_t error_level = 0;

		std::ifstream file(_path);
		if (!file)
		{
			return error_level;
		}

		if (!error_level)
		{
			std::string read_buffer;

			std::vector<vector3> positions;
			std::vector<vector3> normals;
			std::vector<vector2> tex_coords;
			uint32_t index_counter = 0;

			while (file)
			{
				file >> read_buffer;

				if (read_buffer == "v")
				{
					float x = 0.0f, y = 0.0f, z = 0.0f;
					file >> x >> y >> z;
					positions.push_back(vector3(x, y, z));
				}
				else if (read_buffer == "vt")
				{
					float u = 0.0f, v = 0.0f;
					file >> u >> v;
					tex_coords.push_back(vector2(u, v));
				}
				if (read_buffer == "vn")
				{
					float x = 0.0f, y = 0.0f, z = 0.0f;
					file >> x >> y >> z;
					normals.push_back(vector3(x, y, z));
				}
				if (read_buffer == "f")
				{
					while (file.peek() != '\n')
					{
						vertex v;
						uint32_t index = 0;

						file >> index;

						if (!index)
						{
							break;
						}

						v.position = positions[index - 1];

						if (file.peek() == '/')
						{
							file.ignore();

							file >> index;

							v.tex_coord = tex_coords[index - 1];

							if (file.peek() == '/')
							{
								file.ignore();

								file >> index;

								v.normal = normals[index - 1];
							}
						}

						vertices.push_back(v);
						indices.push_back(index_counter);
						++index_counter;
					}
				}
			}
		}

		if (!error_level)
		{
			file.close();
		}

		return error_level;
	}

	int32_t static_mesh::write_native()
	{
		int32_t error_level = 0;

		std::ofstream file(_path, std::ios::out | std::ios::binary);
		if (!file)
		{
			return error_level;			
		}

		if (!error_level)
		{
			file.write("h5stm", 6);

			const size_t vertices_count = vertices.size();
			file.write((const char *)&vertices_count, sizeof(size_t));
			file.write((const char *)vertices.data(), sizeof(vertex) * vertices_count);

			const size_t indices_count = indices.size();
			file.write((const char *)&indices_count, sizeof(size_t));
			file.write((const char *)indices.data(), sizeof(uint32_t) * indices_count);
		}

		if (!error_level)
		{
			file.close();
		}

		return error_level;
	}

	size_t static_mesh::inc_ref()
	{
		if (!refs)
		{
			init();
		}
		++refs;
		return refs;
	}

	size_t static_mesh::dec_ref()
	{
		--refs;
		if (!refs)
		{
			terminate();
		}
		return refs;
	}
}
