dirname = path.dirname(__file__)

world:new_immovable_type{
   name = "skeleton4",
   descname = _ "Seashell",
   editor_category = "miscellaneous",
   size = "none",
   attributes = {},
   programs = {},
   animations = {
      idle = {
         pictures = path.list_files(dirname .. "idle.png"),
         hotspot = { 26, 32 },
      },
   }
}
