// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "FlarmTrafficLook.hpp"
#include "TrafficLook.hpp"
#include "FontDescription.hpp"
#include "Screen/Layout.hpp"

void
FlarmTrafficLook::Initialise(const TrafficLook &other, bool small, bool inverse)
{
  passive_color = Color(0x99, 0x99, 0x99);
  warning_color = other.warning_color;
  alarm_color = other.alarm_color;
  default_color = inverse ? COLOR_WHITE : COLOR_BLACK;
  selection_color = default_color;
  background_color = inverse ? COLOR_BLACK : COLOR_WHITE;
  radar_color = COLOR_GRAY;
  safe_above_color = Color(0x1d,0x9b,0xc5);
  safe_below_color = Color(0x1d,0xc5,0x10);
  warning_in_altitude_range_color = Color(0xff,0x00,0xff);

  warning_brush.Create(warning_color);
  alarm_brush.Create(alarm_color);
  default_brush.Create(default_color);
  passive_brush.Create(passive_color);
  selection_brush.Create(selection_color);
  radar_brush.Create(radar_color);
  team_brush_green.Create(other.team_color_green);
  team_brush_blue.Create(other.team_color_blue);
  team_brush_yellow.Create(other.team_color_yellow);
  team_brush_magenta.Create(other.team_color_magenta);
  safe_above_brush.Create(safe_above_color);
  safe_below_brush.Create(safe_below_color);
  warning_in_altitude_range_brush.Create(warning_in_altitude_range_color);


  unsigned width = Layout::FastScale(small ? 1u : 2u);
  warning_pen.Create(width, warning_color);
  alarm_pen.Create(width, alarm_color);
  default_pen.Create(width, default_color);
  passive_pen.Create(width, passive_color);
  selection_pen.Create(width, selection_color);
  team_pen_green.Create(width, other.team_color_green);
  team_pen_blue.Create(width, other.team_color_blue);
  team_pen_yellow.Create(width, other.team_color_yellow);
  team_pen_magenta.Create(width, other.team_color_magenta);

  plane_pen.Create(width, radar_color);
  radar_pen.Create(1, radar_color);

  unit_fraction_pen.Create(1, inverse ? COLOR_WHITE : COLOR_BLACK);

  no_traffic_font.Load(FontDescription(Layout::FontScale(22)));
  label_font.Load(FontDescription(Layout::FontScale(12)));
  side_info_font.Load(FontDescription(Layout::FontScale(small ? 8 : 12),
                                      true));

  info_labels_font.Load(FontDescription(Layout::FontScale(12), true));
  info_values_font.Load(FontDescription(Layout::FontScale(16)));
  info_units_font.Load(FontDescription(Layout::FontScale(8)));
  call_sign_font.Load(FontDescription(Layout::FontScale(24), true));
}
