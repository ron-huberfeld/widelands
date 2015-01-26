dirname = path.dirname(__file__)

tribes:new_productionsite_type {
   name = "atlanteans_armorsmithy",
   -- TRANSLATORS: This is a building name used in lists of buildings
   descname = _"Armor Smithy",
   size = "medium",

   buildcost = {
		log = 2,
		granite = 2,
		planks = 2,
		quartz = 1
	},
	return_on_dismantle = {
		granite = 1,
		planks = 1,
		quartz = 1
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
		-- #TRANSLATORS: Performance helptext for a building
		performance = _"Calculation needed"
   }

   animations = {
		idle = {
			pictures = { dirname .. "idle_\\d+.png" },
			hotspot = { 53, 60 },
		},
		working = {
			pictures = { dirname .. "idle_\\d+.png" }, -- TODO(GunChleoc): No animation yet.
			hotspot = { 53, 60 },
		}
	},

   aihints = {
		prohibited_till = 900
   },

	working_positions = {
		atlanteans_armorsmith = 1
	},

   inputs = {
		iron = 8,
		gold = 8,
		coal = 8
	},
   outputs = {
		"shield_advanced",
		"shield_steel"
   },

	programs = {
		work = {
			-- TRANSLATORS: Completed/Skipped/Did not start working because ...
			descname = _"working",
			actions = {
				"call=produce_shield_steel",
				"call=produce_shield_advanced",
				"return=skipped"
			}
		},
		produce_shield_steel = {
			-- TRANSLATORS: Completed/Skipped/Did not start forging a steel shield because ...
			descname = _"forging a steel shield",
			actions = {
				"return=skipped unless economy needs shield_steel",
				"sleep=32000",
				"consume=iron:2 coal:2",
				"animate=working 35000",
				"produce=shield_steel"
			}
		},
		produce_shield_advanced = {
			-- TRANSLATORS: Completed/Skipped/Did not start forging an advanced shield because ...
			descname = _"forging an advanced shield",
			actions = {
				"return=skipped unless economy needs shield_advanced",
				"sleep=32000",
				"consume=iron:2 coal:2 gold",
				"animate=working 45000",
				"produce=shield_advanced"
			}
		},
	},
}
