dirname = path.dirname (__file__)

tribes:new_productionsite_type {
   msgctxt = "frisians_building",
   name = "frisians_hunters_house",
   -- TRANSLATORS: This is a building name used in lists of buildings
   descname = pgettext ("frisians_building", "Hunter’s House"),
   helptext_script = dirname .. "helptexts.lua",
   icon = dirname .. "menu.png",
   size = "small",

   buildcost = {
      brick = 1,
      log = 1,
      reed = 1
   },
   return_on_dismantle = {
      brick = 1,
      log = 1
   },

   spritesheets = {
      idle = {
         directory = dirname,
         basename = "idle",
         hotspot = {40, 69},
         frames = 10,
         columns = 5,
         rows = 2,
         fps = 10
      }
   },
   animations = {
      unoccupied = {
         directory = dirname,
         basename = "unoccupied",
         hotspot = {40, 52}
      }
   },

   aihints = {
      collects_ware_from_map = "meat",
      prohibited_till = 480
   },

   indicate_workarea_overlaps = {
      frisians_hunters_house = false,
   },

   working_positions = {
      frisians_hunter = 1
   },

   outputs = {
      "meat",
      "fur"
   },

   programs = {
      work = {
         -- TRANSLATORS: Completed/Skipped/Did not start hunting because ...
         descname = _"hunting",
         actions = {
            "callworker=hunt",
            "sleep=35000",
            "callworker=hunt",
            "sleep=35000",
            "callworker=hunt",
            "sleep=35000",
            "callworker=hunt",
            "sleep=35000",
            "callworker=hunt",
            "sleep=35000",
            "produce=fur"
         }
      },
   },
   out_of_resource_notification = {
      -- Translators: Short for "Out of Game" for a resource
      title = _"No Game",
      -- TRANSLATORS: "Game" means animals that you can hunt
      heading = _"Out of Game",
      -- TRANSLATORS: "game" means animals that you can hunt
      message = pgettext("frisians_building", "The hunter working out of this hunter’s house can’t find any game in his work area."),
      productivity_threshold = 33
   },
}
