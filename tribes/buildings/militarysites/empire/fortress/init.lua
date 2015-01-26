dirname = path.dirname(__file__)

tribes:new_militarysite_type {
   name = "empire_fortress",
   -- TRANSLATORS: This is a building name used in lists of buildings
   descname = _"Fortress",
   size = "big",
   enhancement = "empire_castle",

   buildcost = {
		planks = 5,
		log = 2,
		granite = 8,
		marble = 2,
		marble_column = 2
	},
	return_on_dismantle = {
		planks = 2,
		log = 1,
		granite = 5,
		marble = 1,
		marble_column = 1
	},

   helptexts = {
		-- TRANSLATORS: Lore helptext for a building
		lore = _"Text needed",
		-- TRANSLATORS: Lore author helptext for a building
		lore_author = _"Source needed",
		-- TRANSLATORS: Purpose helptext for a building
		purpose = _"Garrisons soldiers to expand your territory.",
		-- TRANSLATORS: Note helptext for a building
		note = _"If you’re low on soldiers to occupy new military sites, use the downward arrow button to decrease the capacity. You can also click on a soldier to send him away.",
		-- #TRANSLATORS: Performance helptext for a building
		performance = ""
   }

   animations = {
		idle = {
			pictures = { dirname .. "idle_\\d+.png" },
			hotspot = { 90, 105 }
		},
		build = {
			pictures = { dirname .. "build_\\d+.png" },
			hotspot = { 90, 105 }
		}
	},

	outputs = {
		"empire_soldier",
   },

	max_soldiers = 8,
   heal_per_second = 170,
   conquers = 11,
   prefer_heroes = true,

   aihints = {
		expansion = true,
		fighting = true,
		mountain_conqueror = true,
		prohibited_till=1500
   },

   messages = {
		occupied = _"Your soldiers have occupied your fortress.",
		aggressor = _"Your fortress discovered an aggressor.",
		attack = _"Your fortress is under attack.",
		defeated_enemy = _"The enemy defeated your soldiers at the fortress.",
		defeated_you = _"Your soldiers defeated the enemy at the fortress."
   },
}
