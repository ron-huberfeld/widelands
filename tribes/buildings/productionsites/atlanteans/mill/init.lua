dirname = path.dirname(__file__)

tribes:new_productionsite_type {
   name = "atlanteans_mill",
   -- TRANSLATORS: This is a building name used in lists of buildings
   descname = _"Mill",
   size = "medium",

   buildcost = {
		log = 3,
		granite = 3,
		planks = 2
	},
	return_on_dismantle = {
		log = 1,
		granite = 2,
		planks = 1
	},

   helptexts = {
		-- TRANSLATORS: Lore helptext for a building
		lore = _"Text needed",
		-- TRANSLATORS: Lore author helptext for a building
		lore_author = _"Source needed",
		-- TRANSLATORS: Purpose helptext for a building
		purpose = _"Text needed",
		-- #TRANSLATORS: Note helptext for a building
		note = "",
		-- TRANSLATORS: Performance helptext for a building
		performance = _"Calculation needed"
   }

   animations = {
		idle = {
			pictures = { dirname .. "idle_\\d+.png" },
			hotspot = { 58, 61 },
		},
		working = {
			pictures = { dirname .. "working_\\d+.png" },
			hotspot = { 58, 61 },
		}
	},

   aihints = {
		prohibited_till = 600
   },

	working_positions = {
		atlanteans_miller = 1
	},

   inputs = {
		corn = 6,
		blackroot = 6
	},
   outputs = {
		"cornmeal",
		"blackroot_flour"
   },

	programs = {
		work = {
			-- TRANSLATORS: Completed/Skipped/Did not start working because ...
			descname = _"working",
			actions = {
				"call=produce_cornmeal",
				"call=produce_blackroot_flour",
				"return=skipped"
			}
		},
		produce_cornmeal = {
			-- TRANSLATORS: Completed/Skipped/Did not start grinding corn because ...
			descname = _"grinding corn",
			actions = {
				"return=skipped when site has blackroot and economy needs blackroot_flour and not economy needs cornmeal",
				"return=skipped unless economy needs cornmeal",
				"sleep=3500",
				"consume=corn",
				"animate=working 15000",
				"produce=cornmeal"
			}
		},
		produce_shield_advanced = {
			-- TRANSLATORS: Completed/Skipped/Did not start grinding blackrootbecause ...
			descname = _"grinding blackroot",
			actions = {
				-- No check whether we need blackroot_flour because blackroots cannot be used for anything else.
				"return=skipped when site has corn and economy needs cornmeal and not economy needs blackroot_flour",
				"sleep=3500",
				"consume=blackroot",
				"animate=working 15000",
				"produce=blackroot_flour"
			}
		},
	},
}
