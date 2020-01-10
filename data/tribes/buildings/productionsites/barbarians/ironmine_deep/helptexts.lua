function building_helptext_lore()
   -- TRANSLATORS: Lore helptext for a building
   return pgettext("barbarians_building", "‘I look at my own pick wearing away day by day and I realize why my work is important.’")
end

function building_helptext_lore_author()
   -- TRANSLATORS: Lore author helptext for a building
   return pgettext("barbarians_building", "Quote from an anonymous miner.")
end

function building_helptext_purpose()
   -- TRANSLATORS: Purpose helptext for a building
   return pgettext("building", "Digs iron ore out of the ground in mountain terrain.")
end

function building_helptext_note()
   -- TRANSLATORS: Note helptext for a building
   return pgettext("barbarians_building", "This mine exploits only %s of the resource. From there on out, it will only have a 5%% chance of finding any iron ore."):bformat("2/3")
end

function building_helptext_performance()
   -- TRANSLATORS: Performance helptext for a building
   return pgettext("barbarians_building", "If the food supply is steady, this mine can produce iron ore in 39.5 seconds on average.")
end
