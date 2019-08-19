dirname = path.dirname(__file__)

tribes:new_ware_type {
   msgctxt = "ware",
   name = "needles",
   -- TRANSLATORS: This is a ware name used in lists of wares
   descname = pgettext("ware", "Needles"),
   helptext_script = dirname .. "helptexts.lua",
   icon = dirname .. "menu.png",
   default_target_quantity = {
      amazons = 1,
      frisians = 1,
   },
   preciousness = {
      amazons = 1,
      frisians = 0,
   },

   animations = {
      idle = {
         pictures = path.list_files(dirname .. "idle.png"),
         hotspot = { 4, 3 },
      },
   }
}
