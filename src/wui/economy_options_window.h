/*
 * Copyright (C) 2008-2019 by the Widelands Development Team
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

#ifndef WL_WUI_ECONOMY_OPTIONS_WINDOW_H
#define WL_WUI_ECONOMY_OPTIONS_WINDOW_H

#include <map>
#include <memory>
#include <string>

#include "economy/economy.h"
#include "logic/map_objects/tribes/tribe_descr.h"
#include "notifications/notifications.h"
#include "ui_basic/box.h"
#include "ui_basic/dropdown.h"
#include "ui_basic/tabpanel.h"
#include "ui_basic/window.h"
#include "wui/waresdisplay.h"

const std::string kDefaultEconomyProfile = "Default";

struct EconomyOptionsWindow : public UI::Window {
	EconomyOptionsWindow(UI::Panel* parent, Widelands::Economy* economy, bool can_act);
	~EconomyOptionsWindow();

	struct PredefinedTargets {
		using Targets = std::map<Widelands::DescriptionIndex, uint32_t>;
		Targets wares;
		Targets workers;
	};

	void create_target();
	void do_create_target(const std::string&);
	void save_targets();
	void read_targets(const std::string& = kDefaultEconomyProfile);
	void update_profiles(const std::string&);
	std::map<std::string, PredefinedTargets>& get_predefined_targets() {
		return predefined_targets_;
	}
	const PredefinedTargets& get_selected_target() const {
		return predefined_targets_.at(dropdown_.get_selected());
	}

	void change_target(int amount);
	void reset_target();

	void layout() override;

private:
	struct TargetWaresDisplay : public AbstractWaresDisplay {
		TargetWaresDisplay(UI::Panel* const parent,
		                   int32_t const x,
		                   int32_t const y,
		                   Widelands::Serial serial,
		                   Widelands::Player* player,
		                   Widelands::WareWorker type,
		                   bool selectable);

		void set_economy(Widelands::Serial serial);

	protected:
		std::string info_for_ware(Widelands::DescriptionIndex const ware) override;

	private:
		Widelands::Serial serial_;
		Widelands::Player* player_;
	};

	/**
	 * Wraps the wares/workers display together with some buttons
	 */
	struct EconomyOptionsPanel : UI::Box {
		EconomyOptionsPanel(UI::Panel* parent,
		                    EconomyOptionsWindow* eco_window,
		                    Widelands::Serial serial,
		                    Widelands::Player* player,
		                    bool can_act,
		                    Widelands::WareWorker type,
		                    int32_t min_w);

		void set_economy(Widelands::Serial serial);
		void change_target(int amount);
		void reset_target();
		void update_desired_size() override;

	private:
		Widelands::Serial serial_;
		Widelands::Player* player_;
		Widelands::WareWorker type_;
		bool can_act_;
		TargetWaresDisplay display_;
		EconomyOptionsWindow* economy_options_window_;
	};

	/// Actions performed when a NoteEconomyWindow is received.
	void on_economy_note(const Widelands::NoteEconomy& note);

	UI::Box main_box_;
	Widelands::Serial serial_;
	Widelands::Player* player_;
	UI::TabPanel tabpanel_;
	EconomyOptionsPanel* ware_panel_;
	EconomyOptionsPanel* worker_panel_;
	std::unique_ptr<Notifications::Subscriber<Widelands::NoteEconomy>> economynotes_subscriber_;

	std::map<std::string, PredefinedTargets> predefined_targets_;
	UI::Box dropdown_box_;
	UI::Dropdown<std::string> dropdown_;
};

#endif  // end of include guard: WL_WUI_ECONOMY_OPTIONS_WINDOW_H
