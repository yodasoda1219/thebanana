#include "pch.h"
#include "model_registry.h"
#include "internal_util.h"
#include "util.h"
namespace thebanana {
	model_registry::model_registry() {
		this->unload();
	}
	model_registry::model_registry(const std::vector<model_descriptor>& models) : model_registry() {
		this->load(models);
	}
	model_registry::~model_registry() {
		this->unload();
	}
	bool model_registry::loaded() {
		return this->has_loaded;
	}
	void model_registry::reload(const std::vector<model_descriptor>& models) {
		this->unload();
		this->load(models);
	}
	void model_registry::draw(const std::string& name, double time, int m_animation_index) {
		if (!this->has_loaded) return;
		const auto& m = this->models[name];
		if (!m.get()) return;
		m->draw(static_cast<float>(time), m_animation_index);
	}
	transform model_registry::get_transform(const std::string& name) {
		if (!this->has_loaded) return transform();
		std::vector<model_descriptor>::iterator it;
		for (it = this->descriptors.begin(); it != this->descriptors.end(); it++) {
			if (it->name == name) {
				break;
			}
		}
		if (it == this->descriptors.end()) {
			return transform();
		}
		transform model_transform = it->model_transform;
		const aiScene* scene = this->get_scene(name);
		if (scene) model_transform *= scene->mRootNode->mTransformation;
		return model_transform;
	}
	const aiScene* model_registry::get_scene(const std::string& name) {
		if (!this->has_loaded) return NULL;
		const auto& m = this->models[name];
		if (!m.get()) return NULL;
		return m->get_scene();
	}
	void model_registry::add_vertex_data(const std::string& model_name, model_vertex_data data) {
		auto& vertices = this->vertex_data[model_name].vertices;
		vertices.insert(vertices.end(), data.vertices.begin(), data.vertices.end());
		auto& indices = this->vertex_data[model_name].indices;
		indices.insert(indices.end(), data.indices.begin(), data.indices.end());
		auto& bone_data = this->vertex_data[model_name].bone_data;
		bone_data.insert(bone_data.end(), data.bone_data.begin(), data.bone_data.end());
	}
	model_registry::model_vertex_data model_registry::get_vertex_data(const std::string& model_name) {
		if (!this->has_loaded) return model_vertex_data();
		return this->vertex_data[model_name];
	}
	std::string model_registry::path_helper(const std::string& original, const std::string& find, const std::string& replace) {
		size_t pos = std::string::npos;
		std::string result = original;
		do {
			if (pos != std::string::npos) {
				result = replace + result.substr(pos + find.length());
			}
			pos = result.find(find, pos + 1);
		} while (pos != std::string::npos);
		return result;
	}
	struct vertex_loader_params {
		model_registry* r;
		std::string n;
	};
	void vertex_loader(aiMesh*, aiNode*, model*, const std::vector<graphics::mesh::vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<graphics::mesh::vertex_bone_data>& bone_data, vertex_loader_params* params) {
		params->r->add_vertex_data(params->n, { vertices, indices, bone_data });
	}
	void model_registry::load(const std::vector<model_descriptor>& models) {
		for (auto m : models) {
			vertex_loader_params* vlp = new vertex_loader_params{ this, m.name };
			set_per_mesh_function((per_mesh_function)vertex_loader);
			set_user_data(vlp);
			this->vertex_data[m.name] = model_vertex_data();
			this->models[m.name] = std::unique_ptr<model>(new model(m.path));
			delete vlp;
		}
		append_vector(this->descriptors, models);
		this->has_loaded = true;
	}
	std::vector<std::string> model_registry::get_loaded_model_names() const {
		std::vector<std::string> names;
		for (auto& m : this->models) {
			if (m.second.get()) names.push_back(m.first);
		}
		return names;
	}
	void model_registry::unload() {
		this->models.clear();
		this->has_loaded = false;
	}
}