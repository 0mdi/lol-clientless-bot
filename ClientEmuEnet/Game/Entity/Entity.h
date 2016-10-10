#pragma once
#include <string>
#include <stdint.h>

#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>

class Entity
{
public:
	Entity(uint32_t netId, std::string name, std::string type)
		: m_netId(netId)
		, m_name(name)
		, m_type(type)
	{}

	virtual ~Entity() {}

	static luabind::scope registerLuaFunctions()
	{
		return
			luabind::class_<Entity>("Entity")
			.def(luabind::constructor<uint32_t, const char*, const char*>())
			.def("getNetId", &Entity::getNetId)
			.def("getName", &Entity::getName)
			.def("getType", &Entity::getType)
			.def("getX", &Entity::getX)
			.def("getY", &Entity::getY)
			.def("getZ", &Entity::getZ)
			;
	}

	uint32_t getNetId() { return m_netId; }
	std::string getName() { return m_name; }
	std::string getType() { return m_type; }
	float getX() { return m_x; }
	float getY() { return m_y; }
	float getZ() { return m_z; }

	void setX(float x) { m_x = x; }
	void setY(float y) { m_y = y; }
	void setZ(float z) { m_z = z; }
private:
	uint32_t m_netId;
	std::string m_name;
	std::string m_type;

	float m_x;
	float m_y;
	float m_z;
};