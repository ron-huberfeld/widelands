/*
 * Copyright (C) 2006-2014 by the Widelands Development Team
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

#include "graphic/gl/terrain_program.h"

#include "base/log.h"
#include "graphic/gl/fields_to_draw.h"
#include "graphic/graphic.h"
#include "graphic/terrain_texture.h"
#include "graphic/texture.h"

namespace  {

using namespace Widelands;

// QuickRef:
// http://www.cs.unh.edu/~cs770/docs/glsl-1.20-quickref.pdf
// Full specification:
// http://www.opengl.org/registry/doc/GLSLangSpec.Full.1.20.8.pdf
// We target OpenGL 2.1 for the desktop here.
const char kTerrainVertexShader[] = R"(
#version 120

// Attributes.
attribute vec2 attr_position;
attribute vec2 attr_texture_position;
attribute float attr_brightness;
attribute vec2 attr_texture_offset;

// Output of vertex shader.
varying float var_brightness;
varying vec2 var_texture_position;
varying vec2 var_texture_offset;

void main() {
	var_texture_position = attr_texture_position;
	var_brightness = attr_brightness;
	var_texture_offset = attr_texture_offset;
	gl_Position = vec4(attr_position, 0., 1.);
}
)";

const char kTerrainFragmentShader[] = R"(
#version 120

uniform sampler2D u_terrain_texture;
uniform vec2 u_texture_dimensions;

varying float var_brightness;
varying vec2 var_texture_position;
varying vec2 var_texture_offset;

void main() {
	vec4 clr = texture2D(u_terrain_texture, var_texture_offset + u_texture_dimensions.xy * fract(var_texture_position));
	clr.rgb *= var_brightness;
	// gl_FragColor = clr;
	gl_FragColor = vec4(vec3(clr), 1.);
}
)";

}  // namespace

TerrainProgram::TerrainProgram() {
	gl_program_.build(kTerrainVertexShader, kTerrainFragmentShader);

	attr_position_ = glGetAttribLocation(gl_program_.object(), "attr_position");
	attr_texture_position_ =
	   glGetAttribLocation(gl_program_.object(), "attr_texture_position");
	attr_brightness_ = glGetAttribLocation(gl_program_.object(), "attr_brightness");
	attr_texture_offset_ = glGetAttribLocation(gl_program_.object(), "attr_texture_offset");

	u_terrain_texture_ = glGetUniformLocation(gl_program_.object(), "u_terrain_texture");
	u_texture_dimensions_ = glGetUniformLocation(gl_program_.object(), "u_texture_dimensions");
}

void TerrainProgram::gl_draw(int gl_texture, float texture_w, float texture_h) {
	glUseProgram(gl_program_.object());

	glEnableVertexAttribArray(attr_position_);
	glEnableVertexAttribArray(attr_texture_position_);
	glEnableVertexAttribArray(attr_brightness_);
	glEnableVertexAttribArray(attr_texture_offset_);

	glBindBuffer(GL_ARRAY_BUFFER, gl_array_buffer_.object());
	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(TerrainProgram::PerVertexData) * vertices_.size(),
	             vertices_.data(),
	             GL_STREAM_DRAW);

	const auto set_attrib_pointer = [](const int vertex_index, int num_items, int offset) {
		glVertexAttribPointer(vertex_index,
		                      num_items,
		                      GL_FLOAT,
		                      GL_FALSE,
		                      sizeof(TerrainProgram::PerVertexData),
		                      reinterpret_cast<void*>(offset));
	};
	set_attrib_pointer(attr_position_, 2, offsetof(PerVertexData, gl_x));
	set_attrib_pointer(attr_brightness_, 1, offsetof(PerVertexData, brightness));
	set_attrib_pointer(attr_texture_position_, 2, offsetof(PerVertexData, texture_x));
	set_attrib_pointer(attr_texture_offset_, 2, offsetof(PerVertexData, texture_offset_x));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUniform2f(u_texture_dimensions_, texture_w, texture_h);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gl_texture);
	glUniform1i(u_terrain_texture_, 0);

	glDrawArrays(GL_TRIANGLES, 0, vertices_.size());

	glDisableVertexAttribArray(attr_position_);
	glDisableVertexAttribArray(attr_texture_position_);
	glDisableVertexAttribArray(attr_brightness_);
	glDisableVertexAttribArray(attr_texture_offset_);
}

void TerrainProgram::draw(const DescriptionMaintainer<TerrainDescription>& terrains,
                          const FieldsToDraw& fields_to_draw) {
	if (vertices_.size() < fields_to_draw.size() * 3) {
		vertices_.reserve(fields_to_draw.size() * 3);
	}
	vertices_.clear();

	GLuint gl_texture = 0;
	float texture_w, texture_h;

	// NOCOM(#sirver): pull out into methods.
	const auto add_vertex =
	   [this](const FieldsToDraw::Field& field, const FloatRect& texture_coordinates) {
		vertices_.emplace_back(field.gl_x,
		                       field.gl_y,
		                       field.brightness,
		                       field.texture_x,
		                       field.texture_y,
		                       texture_coordinates.x,
		                       texture_coordinates.y);
	};

	// NOCOM(#sirver): check that loaded terrain has the correct size
	const auto add_triangle =
	   [add_vertex, &terrains, this, &gl_texture, &texture_w, &texture_h, &fields_to_draw](
	      int terrain, int index1, int index2, int index3) {
		const Texture& texture = terrains.get_unmutable(terrain).get_texture();
		const FloatRect& texture_coordinates = texture.texture_coordinates();
		if (gl_texture != 0 && gl_texture != texture.get_gl_texture()) {
			throw wexception(
			   "Not all terrain textures are in the same texture atlas (i.e. same texture).");
		}
		// NOCOM(#sirver): pull this out?
		gl_texture = texture.get_gl_texture();
		texture_w = texture_coordinates.w;
		texture_h = texture_coordinates.h;

		add_vertex(fields_to_draw.at(index1), texture_coordinates);
		add_vertex(fields_to_draw.at(index2), texture_coordinates);
		add_vertex(fields_to_draw.at(index3), texture_coordinates);
	};

	for (size_t current_index = 0; current_index < fields_to_draw.size(); ++current_index) {
		const FieldsToDraw::Field& field = fields_to_draw.at(current_index);

		// The bottom right neighbor fields_to_draw is needed for both triangles
		// associated with this field. If it is not in fields_to_draw, there is no need to
		// draw any triangles.
		const int brn_index = fields_to_draw.calculate_index(field.fx + (field.fy & 1), field.fy + 1);
		if (brn_index == -1) {
			continue;
		}

		// Down triangle.
		const int bln_index =
		   fields_to_draw.calculate_index(field.fx + (field.fy & 1) - 1, field.fy + 1);
		if (bln_index != -1) {
			add_triangle(field.ter_d, current_index, bln_index, brn_index);
		}

		// Right triangle.
		const int rn_index = fields_to_draw.calculate_index(field.fx + 1, field.fy);
		if (rn_index != -1) {
			add_triangle(field.ter_r, current_index, brn_index, rn_index);
		}
	}

	gl_draw(gl_texture, texture_w, texture_h);
}
