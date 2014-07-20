/*
 * Copyright (C) 2006-2010 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef WL_SCRIPTING_LUA_MAP_H
#define WL_SCRIPTING_LUA_MAP_H

#include <set>

#include "economy/flag.h"
#include "economy/portdock.h"
#include "economy/road.h"
#include "logic/constructionsite.h"
#include "logic/game.h"
#include "logic/militarysite.h"
#include "logic/productionsite.h"
#include "logic/ship.h"
#include "logic/soldier.h"
#include "logic/trainingsite.h"
#include "logic/warehouse.h"
#include "logic/worker.h"
#include "scripting/luna.h"
#include "third_party/eris/lua.hpp"


namespace Widelands {
	struct Soldier_Descr;
	struct Building_Descr;
	struct WareDescr;
	class Worker_Descr;
	class Bob;
}

namespace LuaMap {

/*
 * Base class for all classes in wl.map
 */
class L_MapModuleClass : public LunaClass {
	public:
		const char * get_modulename() override {return "map";}
};


class L_Map : public L_MapModuleClass {
public:
	LUNA_CLASS_HEAD(L_Map);

	virtual ~L_Map() {}

	L_Map() {}
	L_Map(lua_State * L) {
		report_error(L, "Cannot instantiate a 'Map' directly!");
	}

	virtual void __persist(lua_State * L) override;
	virtual void __unpersist(lua_State * L) override;

	/*
	 * Properties
	 */
	int get_width(lua_State *);
	int get_height(lua_State *);
	int get_player_slots(lua_State *);

	/*
	 * Lua methods
	 */
	int place_immovable(lua_State *);
	int get_field(lua_State *);
	int recalculate(lua_State *);

	/*
	 * C methods
	 */
private:
};


class L_MapObjectDescription : public L_MapModuleClass {
public:
	LUNA_CLASS_HEAD(L_MapObjectDescription);

	virtual ~L_MapObjectDescription() {}

	L_MapObjectDescription() : mapobjectdescr_(nullptr) {}
	L_MapObjectDescription(const Widelands::Map_Object_Descr* const mapobjectdescr)
		: mapobjectdescr_(mapobjectdescr) {}
	L_MapObjectDescription(lua_State* L) : mapobjectdescr_(nullptr) {
		report_error(L, "Cannot instantiate a 'MapObjectDescription' directly!");
	}

	virtual void __persist(lua_State * L) override;
	virtual void __unpersist(lua_State * L) override;

	/*
	 * Properties
	 */
	int get_descname(lua_State *);
	int get_name(lua_State *);
	int get_representative_image(lua_State *);
	int get_type(lua_State *);

	/*
	 * Lua methods
	 */

	/*
	 * C methods
	 */
protected:
	const Widelands::Map_Object_Descr* get() const {
		assert(mapobjectdescr_ != nullptr);
		return mapobjectdescr_;
	}
	// For persistence.
	void set_description_pointer(const Widelands::Map_Object_Descr* pointer) {
		mapobjectdescr_ = pointer;
	}

private:
	const Widelands::Map_Object_Descr* mapobjectdescr_;
};

#define CASTED_GET_DESCRIPTION(klass)                                                              \
	const Widelands::klass* get() const {                                                           \
		return static_cast<const Widelands::klass*>(L_MapObjectDescription::get());                  \
	}

class L_BuildingDescription : public L_MapObjectDescription {
public:
	LUNA_CLASS_HEAD(L_BuildingDescription);

	virtual ~L_BuildingDescription() {}

	L_BuildingDescription() {}
	L_BuildingDescription(const Widelands::Building_Descr* const buildingdescr)
	   : L_MapObjectDescription(buildingdescr) {
	}
	L_BuildingDescription(lua_State* L) : L_MapObjectDescription(L) {
	}

	virtual void __persist(lua_State * L) override;
	virtual void __unpersist(lua_State * L) override;

	/*
	 * Properties
	 */
	int get_build_cost(lua_State *);
	int get_buildable(lua_State *);
	int get_conquers(lua_State *);
	int get_destructible(lua_State *);
	int get_enhanced(lua_State *);
	int get_enhancement_cost(lua_State *);
	int get_enhancement(lua_State *);
	int get_is_mine(lua_State *);
	int get_is_port(lua_State *);
	int get_isproductionsite(lua_State *);
	int get_returned_wares(lua_State *);
	int get_returned_wares_enhanced(lua_State *);
	int get_size(lua_State *);
	int get_vision_range(lua_State *);
	int get_workarea_radius(lua_State *);

	/*
	 * Lua methods
	 */

	/*
	 * C methods
	 */

private:
	CASTED_GET_DESCRIPTION(Building_Descr)
};


class L_ProductionSiteDescription : public L_BuildingDescription {
public:
	LUNA_CLASS_HEAD(L_ProductionSiteDescription);

	virtual ~L_ProductionSiteDescription() {}

	L_ProductionSiteDescription() {}
	L_ProductionSiteDescription(const Widelands::ProductionSite_Descr* const productionsitedescr)
	   : L_BuildingDescription(productionsitedescr) {
	}
	L_ProductionSiteDescription(lua_State* L) : L_BuildingDescription(L) {
	}

	/*
	 * Properties
	 */
	int get_inputs(lua_State *);
	int get_output_ware_types(lua_State *);
	int get_working_positions(lua_State *);

	/*
	 * Lua methods
	 */

	/*
	 * C methods
	 */

private:
	CASTED_GET_DESCRIPTION(ProductionSite_Descr)
};


class L_MilitarySiteDescription : public L_ProductionSiteDescription {
public:
	LUNA_CLASS_HEAD(L_MilitarySiteDescription);

	virtual ~L_MilitarySiteDescription() {}

	L_MilitarySiteDescription() {}
	L_MilitarySiteDescription(const Widelands::ProductionSite_Descr* const militarysitedescr)
	   : L_ProductionSiteDescription(militarysitedescr) {
	}
	L_MilitarySiteDescription(lua_State* L) : L_ProductionSiteDescription(L) {
	}

	/*
	 * Properties
	 */
	int get_heal_per_second(lua_State *);
	int get_max_number_of_soldiers(lua_State *);

	/*
	 * Lua methods
	 */

	/*
	 * C methods
	 */

private:
	CASTED_GET_DESCRIPTION(MilitarySite_Descr)
};


class L_TrainingSiteDescription : public L_ProductionSiteDescription {
public:
	LUNA_CLASS_HEAD(L_TrainingSiteDescription);

	virtual ~L_TrainingSiteDescription() {}

	L_TrainingSiteDescription() {}
	L_TrainingSiteDescription(const Widelands::ProductionSite_Descr* const trainingsitedescr)
	   : L_ProductionSiteDescription(trainingsitedescr) {
	}
	L_TrainingSiteDescription(lua_State* L) : L_ProductionSiteDescription(L) {
	}

	/*
	 * Properties
	 */
	int get_max_attack(lua_State *);
	int get_max_defense(lua_State *);
	int get_max_evade(lua_State *);
	int get_max_hp(lua_State *);
	int get_max_number_of_soldiers(lua_State *);
	int get_min_attack(lua_State *);
	int get_min_defense(lua_State *);
	int get_min_evade(lua_State *);
	int get_min_hp(lua_State *);

	/*
	 * Lua methods
	 */

	/*
	 * C methods
	 */

private:
	CASTED_GET_DESCRIPTION(TrainingSite_Descr)
};



class L_WarehouseDescription : public L_BuildingDescription {
public:
	LUNA_CLASS_HEAD(L_WarehouseDescription);

	virtual ~L_WarehouseDescription() {}

	L_WarehouseDescription() {}
	L_WarehouseDescription(const Widelands::Warehouse_Descr* const warehousedescr)
	   : L_BuildingDescription(warehousedescr) {
	}
	L_WarehouseDescription(lua_State* L) : L_BuildingDescription(L) {
	}

	/*
	 * Properties
	 */
	int get_heal_per_second(lua_State *);

	/*
	 * Lua methods
	 */

	/*
	 * C methods
	 */

private:
	CASTED_GET_DESCRIPTION(Warehouse_Descr)
};



class L_WareDescription : public L_MapObjectDescription {
public:
	LUNA_CLASS_HEAD(L_WareDescription);

	virtual ~L_WareDescription() {}

	L_WareDescription()  {}
	L_WareDescription(const Widelands::WareDescr* const waredescr)
		: L_MapObjectDescription(waredescr) {}
	L_WareDescription(lua_State* L) : L_MapObjectDescription(L) {
	}

	virtual void __persist(lua_State * L) override;
	virtual void __unpersist(lua_State * L) override;

	/*
	 * Properties
	 */
	int get_producers(lua_State *);
	int get_consumers(lua_State *);

	/*
	 * Lua methods
	 */

	/*
	 * C methods
	 */

private:
	CASTED_GET_DESCRIPTION(WareDescr)
};


class L_WorkerDescription : public L_MapObjectDescription {
public:
	LUNA_CLASS_HEAD(L_WorkerDescription);

	virtual ~L_WorkerDescription() {}

	L_WorkerDescription() {}
	L_WorkerDescription(const Widelands::Worker_Descr* const workerdescr)
	   : L_MapObjectDescription(workerdescr) {
	}
	L_WorkerDescription(lua_State* L) : L_MapObjectDescription(L) {
	}

	virtual void __persist(lua_State * L) override;
	virtual void __unpersist(lua_State * L) override;

	/*
	 * Properties
	 */
	int get_becomes(lua_State*);
	int get_level_experience(lua_State*);
	int get_buildable(lua_State*);
	int get_buildcost(lua_State*);

	/*
	 * Lua methods
	 */

	/*
	 * C methods
	 */

private:
	CASTED_GET_DESCRIPTION(Worker_Descr)
};

#undef CASTED_GET_DESCRIPTION

#define CASTED_GET(klass) \
Widelands:: klass * get(lua_State * L, Widelands::Editor_Game_Base & egbase) { \
	return static_cast<Widelands:: klass *> \
		(L_MapObject::get(L, egbase, #klass)); \
}

class L_MapObject : public L_MapModuleClass {
	Widelands::Object_Ptr m_ptr;

public:
	LUNA_CLASS_HEAD(L_MapObject);

	L_MapObject() : m_ptr(nullptr) {}
	L_MapObject(Widelands::Map_Object & mo) {
		m_ptr = &mo;
	}
	L_MapObject(lua_State * L) : m_ptr(nullptr) {
		report_error(L, "Cannot instantiate a '%s' directly!", className);
	}
	virtual ~L_MapObject() {
		m_ptr = nullptr;
	}

	virtual void __persist(lua_State * L) override;
	virtual void __unpersist(lua_State * L) override;

	/*
	 * attributes
	 */
	int get___hash(lua_State *);
	int get_serial(lua_State *);
	int get_type(lua_State *);
	int get_name(lua_State *);
	int get_descname(lua_State *);

	/*
	 * Lua Methods
	 */
	int __eq(lua_State * L);
	int remove(lua_State * L);
	int destroy(lua_State * L);
	int has_attribute(lua_State * L);

	/*
	 * C Methods
	 */
	Widelands::Map_Object * get
		(lua_State *, Widelands::Editor_Game_Base &, std::string = "MapObject");
	Widelands::Map_Object * m_get_or_zero(Widelands::Editor_Game_Base &);
};


class L_BaseImmovable : public L_MapObject {
public:
	LUNA_CLASS_HEAD(L_BaseImmovable);

	L_BaseImmovable() {}
	L_BaseImmovable(Widelands::BaseImmovable & mo) : L_MapObject(mo) {}
	L_BaseImmovable(lua_State * L) : L_MapObject(L) {}
	virtual ~L_BaseImmovable() {}

	/*
	 * Properties
	 */
	int get_size(lua_State * L);
	int get_fields(lua_State * L);

	/*
	 * Lua Methods
	 */

	/*
	 * C Methods
	 */
	CASTED_GET(BaseImmovable)
};

class L_PlayerImmovable : public L_BaseImmovable {
public:
	LUNA_CLASS_HEAD(L_PlayerImmovable);

	L_PlayerImmovable() {}
	L_PlayerImmovable(Widelands::PlayerImmovable & mo) : L_BaseImmovable(mo) {
	}
	L_PlayerImmovable(lua_State * L) : L_BaseImmovable(L) {}
	virtual ~L_PlayerImmovable() {}

	/*
	 * Properties
	 */
	int get_owner(lua_State * L);
	int get_debug_economy(lua_State* L);

	/*
	 * Lua Methods
	 */

	/*
	 * C Methods
	 */
	CASTED_GET(PlayerImmovable)
};

class L_PortDock : public L_PlayerImmovable {
public:
	LUNA_CLASS_HEAD(L_PortDock);

	L_PortDock() {}
	L_PortDock(Widelands::PortDock & mo) : L_PlayerImmovable(mo) {
	}
	L_PortDock(lua_State * L) : L_PlayerImmovable(L) {}
	virtual ~L_PortDock() {}

	/*
	 * Properties
	 */

	/*
	 * Lua methods
	 */

	/*
	 * C methods
	 */
	CASTED_GET(PortDock)
};

class L_Building : public L_PlayerImmovable {
public:
	LUNA_CLASS_HEAD(L_Building);

	L_Building() {}
	L_Building(Widelands::Building & mo) : L_PlayerImmovable(mo) {
	}
	L_Building(lua_State * L) : L_PlayerImmovable(L) {}
	virtual ~L_Building() {}

	/*
	 * Properties
	 */
	int get_flag(lua_State* L);

	/*
	 * Lua Methods
	 */

	/*
	 * C Methods
	 */
	CASTED_GET(Building)
};

class L_Flag : public L_PlayerImmovable {
public:
	LUNA_CLASS_HEAD(L_Flag);

	L_Flag() {}
	L_Flag(Widelands::Flag & mo) : L_PlayerImmovable(mo) {
	}
	L_Flag(lua_State * L) : L_PlayerImmovable(L) {}
	virtual ~L_Flag() {}

	/*
	 * Properties
	 */

	/*
	 * Lua Methods
	 */
	int set_wares(lua_State *);
	int get_wares(lua_State *);

	/*
	 * C Methods
	 */
	CASTED_GET(Flag)
};

class L_Road : public L_PlayerImmovable {
public:
	LUNA_CLASS_HEAD(L_Road);

	L_Road() {}
	L_Road(Widelands::Road & mo) : L_PlayerImmovable(mo) {
	}
	L_Road(lua_State * L) : L_PlayerImmovable(L) {}
	virtual ~L_Road() {}

	/*
	 * Properties
	 */
	int get_end_flag(lua_State * L);
	int get_length(lua_State * L);
	int get_road_type(lua_State * L);
	int get_start_flag(lua_State * L);
	int get_valid_workers(lua_State * L);

	/*
	 * Lua Methods
	 */
	int get_workers(lua_State* L);
	int set_workers(lua_State* L);

	/*
	 * C Methods
	 */
	CASTED_GET(Road)
	static int create_new_worker
			(Widelands::PlayerImmovable &,
			 Widelands::Editor_Game_Base &, const Widelands::Worker_Descr *);
};


class L_ConstructionSite : public L_Building
{
public:
	LUNA_CLASS_HEAD(L_ConstructionSite);

	L_ConstructionSite() {}
	L_ConstructionSite(Widelands::ConstructionSite & mo) : L_Building(mo) {
	}
	L_ConstructionSite(lua_State * L) : L_Building(L) {}
	virtual ~L_ConstructionSite() {}

	/*
	 * Properties
	 */
	int get_building(lua_State *);

	/*
	 * Lua Methods
	 */

	/*
	 * C Methods
	 */
	CASTED_GET(ConstructionSite)
};


class L_Warehouse : public L_Building
{
public:
	LUNA_CLASS_HEAD(L_Warehouse);

	L_Warehouse() {}
	L_Warehouse(Widelands::Warehouse & mo) : L_Building(mo) {
	}
	L_Warehouse(lua_State * L) : L_Building(L) {}
	virtual ~L_Warehouse() {}

	/*
	 * Properties
	 */
	int get_portdock(lua_State* L);

	/*
	 * Lua Methods
	 */
	int get_wares(lua_State*);
	int get_workers(lua_State*);
	int set_wares(lua_State*);
	int set_workers(lua_State*);
	int set_soldiers(lua_State*);
	int get_soldiers(lua_State*);

	/*
	 * C Methods
	 */
	CASTED_GET(Warehouse)
};


class L_ProductionSite : public L_Building {
public:
	LUNA_CLASS_HEAD(L_ProductionSite);

	L_ProductionSite() {}
	L_ProductionSite(Widelands::ProductionSite & mo) : L_Building(mo) {
	}
	L_ProductionSite(lua_State * L) : L_Building(L) {}
	virtual ~L_ProductionSite() {}

	/*
	 * Properties
	 */
	int get_valid_wares(lua_State * L);
	int get_valid_workers(lua_State* L);

	/*
	 * Lua Methods
	 */
	int get_wares(lua_State * L);
	int get_workers(lua_State* L);
	int set_wares(lua_State * L);
	int set_workers(lua_State* L);

	/*
	 * C Methods
	 */
	CASTED_GET(ProductionSite)
	static int create_new_worker
		(Widelands::PlayerImmovable &, Widelands::Editor_Game_Base &,
		 const Widelands::Worker_Descr *);
};

class L_MilitarySite : public L_Building {
public:
	LUNA_CLASS_HEAD(L_MilitarySite);

	L_MilitarySite() {}
	L_MilitarySite(Widelands::MilitarySite & mo) : L_Building(mo) {
	}
	L_MilitarySite(lua_State * L) : L_Building(L) {}
	virtual ~L_MilitarySite() {}

	/*
	 * Properties
	 */
	int get_max_soldiers(lua_State*);

	/*
	 * Lua Methods
	 */
	int set_soldiers(lua_State*);
	int get_soldiers(lua_State*);

	/*
	 * C Methods
	 */
	CASTED_GET(MilitarySite)
};


class L_TrainingSite : public L_ProductionSite {
public:
	LUNA_CLASS_HEAD(L_TrainingSite);

	L_TrainingSite() {}
	L_TrainingSite(Widelands::TrainingSite & mo) : L_ProductionSite(mo) {
	}
	L_TrainingSite(lua_State * L) : L_ProductionSite(L) {}
	virtual ~L_TrainingSite() {}

	/*
	 * Properties
	 */
	int get_max_soldiers(lua_State*);

	/*
	 * Lua Methods
	 */
	int set_soldiers(lua_State*);
	int get_soldiers(lua_State*);

	/*
	 * C Methods
	 */
	CASTED_GET(TrainingSite)
};

class L_Bob : public L_MapObject {
public:
	LUNA_CLASS_HEAD(L_Bob);

	L_Bob() {}
	L_Bob(Widelands::Bob & mo) : L_MapObject(mo) {}
	L_Bob(lua_State * L) : L_MapObject(L) {}
	virtual ~L_Bob() {}

	/*
	 * Properties
	 */
	int has_caps(lua_State *);

	/*
	 * Lua Methods
	 */

	/*
	 * C Methods
	 */
	CASTED_GET(Bob)
};

class L_Worker : public L_Bob {
public:
	LUNA_CLASS_HEAD(L_Worker);

	L_Worker() {}
	L_Worker(Widelands::Worker & w) : L_Bob(w) {}
	L_Worker(lua_State * L) : L_Bob(L) {}
	virtual ~L_Worker() {}

	/*
	 * Properties
	 */
	int get_owner(lua_State * L);
	int get_location(lua_State *);

	/*
	 * Lua methods
	 */

	/*
	 * C methods
	 */
	CASTED_GET(Worker)
};

class L_Soldier : public L_Worker {
public:
	LUNA_CLASS_HEAD(L_Soldier);

	L_Soldier() {}
	L_Soldier(Widelands::Soldier & w) : L_Worker(w) {}
	L_Soldier(lua_State * L) : L_Worker(L) {}
	virtual ~L_Soldier() {}

	/*
	 * Properties
	 */
	int get_attack_level(lua_State *);
	int get_defense_level(lua_State *);
	int get_hp_level(lua_State *);
	int get_evade_level(lua_State *);

	/*
	 * Lua methods
	 */

	/*
	 * C methods
	 */
	CASTED_GET(Soldier)
};

class L_Ship : public L_Bob {
public:
	LUNA_CLASS_HEAD(L_Ship);

	L_Ship() {}
	L_Ship(Widelands::Ship& s) : L_Bob(s) {}
	L_Ship(lua_State* L) : L_Bob(L) {}
	virtual ~L_Ship() {}

	/*
	 * Properties
	 */
	int get_debug_economy(lua_State * L);
	int get_last_portdock(lua_State* L);
	int get_destination(lua_State* L);

	/*
	 * Lua methods
	 */
	int get_wares(lua_State* L);
	int get_workers(lua_State* L);

	/*
	 * C methods
	 */
	CASTED_GET(Ship)
};
#undef CASTED_GET

class L_Field : public L_MapModuleClass {
	Widelands::Coords m_c;
public:
	LUNA_CLASS_HEAD(L_Field);

	L_Field() {}
	L_Field (Widelands::Coordinate x, Widelands::Coordinate y) :
		m_c(Widelands::Coords(x, y)) {}
	L_Field (Widelands::Coords c) : m_c(c) {}
	L_Field(lua_State * L) {
		report_error(L, "Cannot instantiate a 'Field' directly!");
	}
	virtual ~L_Field() {}

	virtual void __persist(lua_State * L) override;
	virtual void __unpersist(lua_State * L) override;

	/*
	 * Properties
	 */
	int get___hash(lua_State *);
	int get_x(lua_State * L);
	int get_y(lua_State * L);
	int get_viewpoint_x(lua_State * L);
	int get_viewpoint_y(lua_State * L);
	int get_height(lua_State * L);
	int set_height(lua_State * L);
	int get_raw_height(lua_State * L);
	int set_raw_height(lua_State * L);
	int get_immovable(lua_State * L);
	int get_bobs(lua_State * L);
	int get_terr(lua_State * L);
	int set_terr(lua_State * L);
	int get_terd(lua_State * L);
	int set_terd(lua_State * L);
	int get_rn(lua_State *);
	int get_ln(lua_State *);
	int get_trn(lua_State *);
	int get_tln(lua_State *);
	int get_bln(lua_State *);
	int get_brn(lua_State *);
	int get_resource(lua_State *);
	int set_resource(lua_State *);
	int get_resource_amount(lua_State *);
	int set_resource_amount(lua_State *);
	int get_claimers(lua_State *);
	int get_owner(lua_State *);

	/*
	 * Lua methods
	 */
	int __tostring(lua_State * L);
	int __eq(lua_State * L);
	int region(lua_State * L);
	int has_caps(lua_State *);

	/*
	 * C methods
	 */
	inline const Widelands::Coords & coords() {return m_c;}
	const Widelands::FCoords fcoords(lua_State * L);

private:
	int m_region(lua_State * L, uint32_t radius);
	int m_hollow_region(lua_State * L, uint32_t radius, uint32_t inner_radius);
};

class L_PlayerSlot : public L_MapModuleClass {
	Widelands::Player_Number m_plr;

public:
	LUNA_CLASS_HEAD(L_PlayerSlot);

	L_PlayerSlot() : m_plr(0) {}
	L_PlayerSlot(Widelands::Player_Number plr) : m_plr(plr) {}
	L_PlayerSlot(lua_State * L) : m_plr(0) {
		report_error(L, "Cannot instantiate a 'PlayerSlot' directly!");
	}
	virtual ~L_PlayerSlot() {}

	virtual void __persist(lua_State * L) override;
	virtual void __unpersist(lua_State * L) override;

	/*
	 * Properties
	 */
	int get_tribe_name(lua_State *);
	int get_name(lua_State *);
	int get_starting_field(lua_State *);

	/*
	 * Lua methods
	 */

	/*
	 * C methods
	 */
};

int upcasted_building_descr_to_lua(lua_State* L, const Widelands::Building_Descr* descr);
int upcasted_immovable_to_lua(lua_State * L, Widelands::BaseImmovable * bi);
int upcasted_bob_to_lua(lua_State * L, Widelands::Bob * mo);

void luaopen_wlmap(lua_State *);

}  // namespace LuaMap

#endif  // end of include guard: WL_SCRIPTING_LUA_MAP_H
