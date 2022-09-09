#include "env.h"

#define DEFAULT_TEXTURE "texture_map.bmp"

using namespace std;

Env::Env() :SDL_texture_map(NULL), texture_map(NULL) {
	blocks.clear();
	visible_faces.clear();
	entities.clear();
	load_texture(DEFAULT_TEXTURE);
};

void Env::load_texture(const char* image_path) {
	SDL_texture_map = IMG_Load(image_path);
	if (SDL_texture_map == NULL) {
		exit(1);
	}
	texture_map = (Uint32(*)[TEXTURE_WIDTH]) SDL_texture_map->pixels;
}

void Env::create_block(const Vector& pos) {
	if (blocks.find(pos) == blocks.end()) {
		Block* block = new Block(pos);
		blocks[pos] = block;
		update_visible_faces();
	}
}

void Env::remove_block(const Vector& pos) {
	if (blocks.find(pos) != blocks.end()) {
		Block*& block = blocks[pos];
		delete block;
		blocks.erase(pos);
		update_visible_faces();
	}
}

Object& Env::create_entity(const Vector& pos) {
	Object* entity = new Object(pos);
	entities.insert(entity);
	update_visible_faces();
	return *entity;
}

void Env::remove_entity(Object*& entity) {
	delete entity;
	entities.erase(entity);
}

void Env::update_visible_faces() {
	// should be optimized so that we only update the necessary faces
	visible_faces.clear();
	for (const auto& pos_block : blocks) {
		const Vector& pos = pos_block.first;
		Block& block = *(pos_block.second);
		for (int i = 0; i < block.faces_len; i++) {
			if (blocks.find(pos + block[i].normal * 2) == blocks.end())
				visible_faces[&block[i]] = &block;
		}
	}

	for (Object* const& entity_ptr : entities) {
		Object& entity = *entity_ptr;
		for (int i = 0; i < entity.faces_len; i++) {
			visible_faces[&entity[i]] = &entity;
		}
	}
}

Env::~Env() {
	free_blocks();
	free_entities();
}

void Env::free_blocks() {
	for (auto& pos_block : blocks) {
		Block* block = pos_block.second;
		delete block;
	}
}

void Env::free_entities() {
	for (Object* const& entity : entities) {
		delete entity;
	}
}

SCALAR Env::object_env_collision(const Object& object, const Vector& v,
	Vector& normal, Object*& collision) {
	SCALAR min_t = SCALAR_MAX;
	SCALAR t;
	Vector curr_normal;
	for (auto face_object : visible_faces) {
		Face& face = *face_object.first;
		Object& other = *face_object.second;

		if (distance(object.pos, other.pos) > norm(v) + 4 ||
			face_object.second == &object) {
			continue;
		}
		for (int i = 0; i < object.faces_len; i++) {
			t = face_face_collision(object[i], face, v, curr_normal);
			if (t < min_t) {
				min_t = t;
				normal = curr_normal;
				collision = face_object.second;
			}
		}
	}
	return min_t;
}

void Env::player_interact(Object& player, Vector& direction,
	PlayerInteraction interaction) {
	Vector normal;
	Object* collision = nullptr;
	if (point_env_collision(player.pos, (direction * REACH_DIST), normal,
		collision) < SCALAR_MAX) {
		Vector block_pos;
		switch (interaction) {
		case ADD_BLOCK:
			block_pos = collision->pos + 2 * normal;
			create_block(block_pos);
			break;
		case REMOVE_BLOCK:
			block_pos = collision->pos;
			remove_block(block_pos);
			break;
		}
	}
}

SCALAR Env::point_env_collision(const Vector& pos, const Vector& v,
	Vector& normal, Object*& collision) {
	SCALAR min_t = SCALAR_MAX;
	SCALAR t;
	Vector curr_normal;
	for (auto face_object : visible_faces) {
		Face& face = *face_object.first;

		if (face.average_dist(pos) > norm(v) + 2) {
			continue;
		}
		t = point_face_collision(pos, face, v, curr_normal);
		if (t < min_t) {
			min_t = t;
			normal = curr_normal;
			collision = face_object.second;
		}
	}
	return min_t;
}

// Vector offset;
// Vector block_pos;
// Vector curr_normal;
// Vector rounded = get_block_pos(object.pos);
// SCALAR radius = ceil(norm(v));
// collision = nullptr;
// // should be optimized so that only the relevant block positions are
// // considered
// for (SCALAR x = -radius; x <= radius; x++) {
//   for (SCALAR y = -radius; y <= radius; y++) {
//     for (SCALAR z = -radius; z <= radius; z++) {
//       // could be deleted
//       if (x != 0 || y != 0 || z != 0) {
//         offset = Vector(2 * x, 2 * y, 2 * z);
//         block_pos = rounded + offset;
//         if (blocks.find(block_pos) != blocks.end()) {
//           Object &block = blocks[block_pos];
//           t = block_block_collision(object, block, v, curr_normal);
//           if (t < min_t) {
//             min_t = t;
//             normal = curr_normal;
//             collision = &block;
//           }
//         }
//       }
//     }
//   }
// }

// SCALAR min_t = 1;
// vector<Vector> collisions;
// Vector direction, curr_offset, block_pos, curr_pos, curr_normal;

// SCALAR t;
// for (int axis = 0; axis < AXES; axis++) {
//   if (v[axis] == 0) {
//     continue;
//   }
//   direction = v * (1 / abs(v[axis]));
//   // the reason this thing works is not trivial: since we intersect any of
//   // the blocks in the trajectory only once and from a *single face* we
//   // get that we add every block in the trajectory exactly once
//   for (curr_offset = direction; abs(curr_offset[axis]) < abs(v[axis]) + 1;
//        curr_offset = curr_offset + direction) {
//     curr_pos = object.pos + curr_offset;
//     block_pos = curr_pos;
//     block_pos[axis] += direction[axis];
//     block_pos = get_block_pos(block_pos);
//     collisions.push_back(block_pos);
//   }

//   for (Vector block_pos : collisions) {
//     if (blocks.find(block_pos) != blocks.end()) {
//       Object &block = blocks[block_pos];
//       t = block_block_collision(object, block, v, curr_normal);
//       if (t < min_t) {
//         min_t = t;
//         normal = curr_normal;
//         collision = &block;
//       }
//     }
//   }
// }

// may replace v with object.v
void Env::move_object(Object& object, Vector& v) {
	Vector normal;
	Object* collision = nullptr;
	SCALAR t = object_env_collision(object, v, normal, collision);
	while (t < 1) {
		object.pos = object.pos + v * t;
		SCALAR force = (normal * v) * (t - 1);
		v = ((1 - t) * v + force * normal) * (collision->friction / (force + 1));
		t = object_env_collision(object, v, normal, collision);
	}
	object.pos = object.pos + v;
	object.v *= AIR_FRICTION;
	object.update();
}

void Env::apply_physics(SCALAR frame_time) {
	Vector gravity = GRAVITY * frame_time;
	for (Object* const& entity_ptr : entities) {
		Object& entity = *entity_ptr;
		entity.v += gravity;
		move_object(entity, entity.v);
	}
}