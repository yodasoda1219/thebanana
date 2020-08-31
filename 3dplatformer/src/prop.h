#pragma once
#include "gameobject.h"
class prop : public gameobject {
public:
	prop(const std::string& model_name);
	virtual void update() override;
	virtual void render() override;
	virtual ~prop() override;
	virtual std::string get_model_name() override;
private:
	std::string m_model_name;
};