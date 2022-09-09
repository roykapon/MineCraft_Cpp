#include "renderer.h"

#include <omp.h>
using namespace std;

// =========================== Renderer ===========================

void Renderer::update_world() {
	world = shift(pos * (-1)) * rotate(Y_AXIS, -horizontal) *
		rotate(X_AXIS, -vertical);
}

void Renderer::update() {
	update_world();
	update_direction();
}

void Renderer::init_picture_colored() {
	memset(picture_colored, 0, sizeof(int) * WIDTH * HEIGHT);
}

void Renderer::init_z_buffer() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			z_buffer[y][x] = SCALAR_MAX;
		}
	}
}

void Renderer::project(ColoredVector& v, Pixel& p) {
	// assumes the point was already rotated
	p.source = v;
	const SCALAR ratio = ffd / v.z;
	p.x = COORD_TO_PIXEL_X(v.x * ratio);
	p.y = COORD_TO_PIXEL_Y(v.y * ratio);
}

void Renderer::update_direction() {
	direction = get_direction(horizontal, vertical);
}

int Renderer::project(Face& face, Pixel* projected) {
	ColoredVector rotated[3];
	int j = 0;
	for (int i = 0; i < 3; i++) {
		rotated[i] = face[i] * world;
	}
	for (int i = 0; i < 3; i++) {
		// fisrt point is in front of the camera
		if (rotated[i].z > EPSILON) {
			project(rotated[i], projected[j]);
			j++;
		}
		// one point is in front and one is behind the camera
		if ((rotated[(i + 1) % 3].z <= EPSILON && rotated[i].z > EPSILON) ||
			(rotated[i].z <= EPSILON && rotated[(i + 1) % 3].z > EPSILON)) {
			// find intersection
			const SCALAR ratio = (EPSILON - rotated[(i + 1) % 3].z) /
				(rotated[i].z - rotated[(i + 1) % 3].z);
			ColoredVector inter;
			interpolate(rotated[i], rotated[(i + 1) % 3], ratio, inter);
			// can be optimized (x and y pixel coordinates can be easily calculated)
			project(inter, projected[j]);
			j++;
		}
		// else both points are behind the camera (do not add anything)
	}
	return j;
}

void update_edges(const Pixel& p, Pixel(*edges)[2], int relative_y) {
	Pixel(&edge)[2] = edges[relative_y];
	if (p.x < edge[0].x) {
		edge[0] = p;
	}
	if (p.x > edge[1].x) {
		edge[1] = p;
	}
}

void Renderer::save_line(Pixel line[2], Pixel(*edges)[2], int extremum[2]) {
	int line_extremum[2];
	get_extremum(line, 2, line_extremum, 0, HEIGHT);
	const Pixel& p1 = line[0];
	const Pixel& p2 = line[1];
	// handle "edge" cases (get the joke?)
	if (p1.y >= extremum[0] && p1.y <= extremum[1]) {
		update_edges(p1, edges, p1.y - extremum[0]);
	}
	if (p2.y >= extremum[0] && p2.y <= extremum[1]) {
		update_edges(p2, edges, p2.y - extremum[0]);
	}
	for (int y = line_extremum[0] + 1; y < line_extremum[1]; y++) {
		const Pixel curr = interpolate_by_y(p1, p2, y);
		update_edges(curr, edges, y - extremum[0]);
	}
}

void init_edges(Pixel(*edges)[2], int* extremum) {
	for (int relative_y = 0; relative_y <= extremum[1] - extremum[0];
		relative_y++) {
		edges[relative_y][0] = Pixel(INT_MAX, relative_y);
		edges[relative_y][1] = Pixel(INT_MIN, relative_y);
	}
}

bool Renderer::decide_to_render2(Pixel* projected, int len) {
	/** temp_i is true if the face is not out of the picture in the i'th
	 direction (left, right, down, up) */
	bool temp_left = false, temp_right = false, temp_bottom = false,
		temp_top = false, temp_forward = false;
	for (int i = 0; i < len; i++) {
		const Pixel& p = projected[i];
		temp_left |= (p.x > 0);
		temp_right |= (p.x < WIDTH - 1);
		temp_bottom |= (p.y > 0);
		temp_top |= (p.y < HEIGHT - 1);
		temp_forward |= (p.source.z > 2 * EPSILON);
	}
	return temp_left && temp_right && temp_bottom && temp_top && temp_forward;
}

void Renderer::render(Face& face) {
	Pixel projected[4];
	int len = project(face, projected);
	if (!decide_to_render2(projected, len)) {
		return;
	}
	int extremum[2];
	get_extremum(projected, len, extremum, 0, HEIGHT - 1);
	if (extremum[1] - extremum[0] <= 0) {
		// happens only if all points are under the view or above the view
		return;
	}
	// Array of the pixels of the edges of the face on every y coordinate
	Pixel(*edges)[2] = new Pixel[extremum[1] - extremum[0] + 1][2];
	init_edges(edges, extremum);
	Pixel line[2];
	for (int i = 0; i < len; i++) {
		line[0] = projected[i];
		line[1] = projected[(i + 1) % len];
		save_line(line, edges, extremum);
	}

	paint_face(edges, extremum);
	delete[] edges;
}

struct FaceComparator {
	Vector pos;

	FaceComparator(Vector _pos) { pos = _pos; }

	bool operator()(Face* face1, Face* face2) {
		return face1->average_dist(pos) < face2->average_dist(pos);
	}
};

void Renderer::render(Env& env) {
	init_picture();
	init_picture_colored();
	init_z_buffer();

	vector<Face*> faces;
	for (const auto& face_object : env.visible_faces) {
		faces.push_back(face_object.first);
	}

	sort(faces.begin(), faces.end(), FaceComparator(pos));
	for (Face* face : faces) {
		// should probably filter the faces before sorting
		if (decide_to_render(*face)) {
			render(*face);
		}
	}
}

bool Renderer::decide_to_render(Face& face) {
	Vector face_direction = normalized(face[0] - pos);
	/** assert the angle between the ray to the face and the normal is lower than
	 90 degrees (otherwise the face cannot be seen from the camera's position) */
	SCALAR relative_direction = face_direction * face.normal;
	/** assert the angle between the camera's direction and the normal is between
	 * 90 and 270 (otherwise the face cannot be seen from the camera's direction)
	 */
	 // should be changed (it is not enough to look only at a single vertex)
	 // SCALAR relative_direction2 = direction * face_direction;
	return relative_direction < 0; //&& relative_direction2 > 0;
}

int bound(int i, int upper_bound, int lower_bound) {
	return max(min(i, upper_bound), lower_bound);
}

void Renderer::paint_face(Pixel(*edges)[2], int* extremum) {
	//#pragma omp parallel for
	const int final_y = min(extremum[1], HEIGHT - 1);
	for (int y = max(extremum[0], 0); y <= final_y; y++) {
		const int relative_y = y - extremum[0];
		const Pixel& left = edges[relative_y][0];
		const Pixel& right = edges[relative_y][1];
		SCALAR* z_buffer_row = z_buffer[y];
		int* picture_colored_row = picture_colored[y];
		// actual work:
		const SCALAR diff_x = left.source.x - right.source.x;
		const SCALAR diff_z = left.source.z - right.source.z;
		// paint a single horizontal line
		const int final_x = min(right.x, WIDTH - 1);
		for (int x = max(left.x, 0); x <= final_x; x++) {
			// check the z_buffer
			SCALAR& old_z = z_buffer_row[x];
			int& next_x = picture_colored_row[x];
			const SCALAR x_coord = PIXEL_TO_COORD_X(x);
			// assumes ffd = 1!!!
			const SCALAR ratio = (right.source.x - right.source.z * x_coord) /
				((diff_z)*x_coord - diff_x);
			const SCALAR z = INTERPOLATE(left.source.z, right.source.z, ratio);

			if (z < old_z) {
				// paint the pixel
				paint_pixel(left, right, x, y, ratio);
				// set the pixel colored
				next_x = (right.x - x);
				old_z = z;
			}
			else if (z < old_z - EPSILON) {
				x += next_x;
			}
		}
	}
}

Renderer::~Renderer() {
	delete[] picture_colored;
	delete[] z_buffer;
}

Pixel Renderer::interpolate_by_y(const Pixel& p1, const Pixel& p2, int y) {
	// assumes p2.y - p1.y != 0
	SCALAR ratio = (SCALAR)(p2.y - y) / (SCALAR)(p2.y - p1.y);
	Pixel res = Pixel(lround(INTERPOLATE(p1.x, p2.x, ratio)), y);

	SCALAR y_coord = PIXEL_TO_COORD_Y(y);
	// assumes ffd = 1 !!!
	SCALAR ratio_3d =
		(p2.source.y - p2.source.z * y_coord) /
		((p1.source.z - p2.source.z) * y_coord - p1.source.y + p2.source.y);
	interpolate(p1.source, p2.source, ratio_3d, res.source);
	return res;
}