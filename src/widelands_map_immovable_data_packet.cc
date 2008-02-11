/*
 * Copyright (C) 2002-2004, 2006-2008 by the Widelands Development Team
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "widelands_map_immovable_data_packet.h"

#include "editor_game_base.h"
#include "immovable.h"
#include "map.h"
#include "tribe.h"
#include "world.h"
#include "widelands_fileread.h"
#include "widelands_filewrite.h"
#include "widelands_map_data_packet_ids.h"
#include "widelands_map_map_object_loader.h"
#include "widelands_map_map_object_saver.h"

#include <map>

namespace Widelands {

/* VERSION 1: initial release
     - registering through Map_Object_Loader/Saver
     - handling for tribe immovables (ignored on skip)
*/

#define CURRENT_PACKET_VERSION 1


void Map_Immovable_Data_Packet::Read
(FileSystem & fs,
 Editor_Game_Base* egbase,
 const bool skip,
 Map_Map_Object_Loader * const ol)
throw (_wexception)
{
	assert(ol);

	FileRead fr;
	fr.Open(fs, "binary/immovable");

	Map        & map        = egbase->map();
	World      & world      = map.world  ();
	const Extent map_extent = map.extent ();

	try {
		const uint16_t packet_version = fr.Unsigned16();
		if (packet_version == CURRENT_PACKET_VERSION) {
			for (;;) {
				uint32_t const reg = fr.Unsigned32();
				if (reg == 0xffffffff)
					break;
				const char * const owner = fr.CString ();
				const char * const name  = fr.CString ();
				const Coords position    = fr.Coords32(map_extent);

				assert(not ol->is_object_known(reg));

				if (strcmp(owner, "world")) {
					if (not skip) { //  do not load player immovables in normal maps
						//  It is a tribe immovable
						Tribe_Descr const & tribe =
							egbase->manually_load_tribe(owner);
						int32_t idx = tribe.get_immovable_index(name);
						if (idx != -1)
							ol->register_object
								(egbase,
								 reg,
								 &egbase->create_immovable(position, idx, &tribe));
						else
							throw wexception
								("tribe %s does not define immovable type \"%s\"",
								 owner, name);
					}
				} else {
					//  world immovable
					int32_t const idx = world.get_immovable_index(name);
					if (idx != -1) {
						Immovable & immovable =
							egbase->create_immovable(position, idx, 0);
						if (not skip)
							ol->register_object(egbase, reg, &immovable);
					} else
						throw wexception
							("world %s does not define immovable type \"%s\"",
							 world.get_name(), name);
				}
			}
		} else
			throw wexception
				("unknown/unhandled version %u", packet_version);
	} catch (_wexception const & e) {
		throw wexception("reading immovable data: %s", e.what());
	}
}


void Map_Immovable_Data_Packet::Write
(FileSystem &, Editor_Game_Base *, Map_Map_Object_Saver * const)
throw (_wexception)
{
	throw wexception("Immovable_Data_Packet is obsolete");
}

};
