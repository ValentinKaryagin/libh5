#pragma once

#include "component.h"

namespace h5
{
	class scene;

	class transform_component
		: public component
	{
	protected:
		transform_component *root = nullptr;

	protected:
		vector3 translation;
		quat	rotation;
		vector3	scaling;

	protected:
		scene *scene_ptr = nullptr;

	public:
		transform_component();
		virtual ~transform_component();

	public:
		void attach(transform_component *ptr);
		void attach_to(transform_component *ptr);
		void detach(transform_component *ptr);
		void detach();

	public:
		virtual void set_translation(const vector3 &v);
		virtual void set_rotation(const quat &r);
		virtual void set_scale(const vector3 &v);

	public:
		virtual vector3 get_translation() const;
		virtual quat get_rotation() const;
		virtual vector3 get_scaling() const;

	public:
		virtual void add_translation(const vector3 &v);
		virtual void add_rotation(const quat &q);
		virtual void add_scaling(const vector3 &v);

	public:
		virtual vector3 get_forward_direction() const;
		virtual vector3 get_right_direction() const;
		virtual vector3 get_up_direction() const;

	public:
		virtual matrix get_translation_matrix() const;
		virtual matrix get_rotation_matrix() const;
		virtual matrix get_scaling_matrix() const;
		virtual matrix get_transform_matrix() const;

	public:
		virtual void on_insert_to_scene(scene *ptr);
		virtual void on_remove_from_scene(scene *ptr);

	public:
		scene *get_scene();
	};
}
