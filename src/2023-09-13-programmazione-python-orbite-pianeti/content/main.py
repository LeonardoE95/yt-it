#!/usr/bin/env python3

import math
from pyray import *

SCREEN_WIDTH = 1920
SCREEN_HEIGHT = 1080

# Gravitational Constant G, m^3 * kg^-1 * s^-2
G = 6.67428E-11

# Distance between Earth and Sun in meters
AU = 149.6E6 * 1000

# 1AU = 250 pixel
SCALE = 250 / AU

class Object:
    def __init__(self, mass, position, velocity, radius, color, label):
        self.mass = mass
        self.position = position
        self.velocity = velocity
        self.radius = radius
        self.color = color
        self.label = label

# ------------------------

def compute_gforce_vector(obj, other_obj):
    global G

    r_vec = Vector2(
        other_obj.position.x - obj.position.x,
        other_obj.position.y - obj.position.y
    )

    r_mag = math.sqrt(r_vec.x * r_vec.x + r_vec.y * r_vec.y)
    gforce_mag = G * obj.mass * other_obj.mass / (r_mag * r_mag)
    r_unit_vec = Vector2(r_vec.x / r_mag, r_vec.y / r_mag)

    return vector2_scale(r_unit_vec, gforce_mag)

def compute_gforce_vectors(objects):
    saved_gforce_vectors = [Vector2(0, 0)]  * len(objects)

    for i in range(0, len(objects)):
        obj = objects[i]
        
        total_gforce = Vector2(0, 0)
        for other_obj in objects:

            # Skip body gravitational pull towards itself
            if obj is other_obj:
                continue

            gforce_vec = compute_gforce_vector(obj, other_obj)
            total_gforce = vector2_add(total_gforce, gforce_vec)

        saved_gforce_vectors[i] = total_gforce
    
    return saved_gforce_vectors

def apply_force_vectors(objects, saved_force_vectors):
    # compute effect day by day
    timestep = 24 * 3600
    
    for i in range(0, len(objects)):
        obj = objects[i]
        force = saved_force_vectors[i]

        # update velocity
        obj.velocity.x += force.x / obj.mass * timestep
        obj.velocity.y += force.y / obj.mass * timestep

        # update position
        obj.position.x += obj.velocity.x * timestep
        obj.position.y += obj.velocity.y * timestep
        
        objects[i] = obj

    return objects

def step_simulation_gforce(objects):
    # TODO: apply gravitational force by netwon to all objects and
    # update position and velocity

    saved_gforce_vectors = compute_gforce_vectors(objects)
    new_objects = apply_force_vectors(objects, saved_gforce_vectors)
    
    return new_objects

# ------------------------

def render_paths(objects, texture):
    begin_texture_mode(texture)
    for obj in objects:
        scaled_position = Vector2(
            obj.position.x * SCALE + SCREEN_WIDTH / 2,
            obj.position.y * SCALE + SCREEN_HEIGHT / 2
        )
        # draw_pixel_v(scaled_position, WHITE)
        draw_circle_v(scaled_position, 1.5, WHITE)
        
    end_texture_mode()

    draw_texture_rec(texture.texture,
                     Rectangle(0, 0, texture.texture.width, -texture.texture.height),
                     Vector2(0, 0),
                     WHITE)

def render_objects(objects):
    for obj in objects:
        scaled_position = Vector2(
            obj.position.x * SCALE + SCREEN_WIDTH / 2,
            obj.position.y * SCALE + SCREEN_HEIGHT / 2
        )
        draw_circle_v(scaled_position, obj.radius, obj.color)

# ------------------------

def main():
    global SCREEN_WIDTH, SCREEN_HEIGHT, AU
    
    init_window(SCREEN_WIDTH, SCREEN_HEIGHT, "GForce Simulation")
    set_window_position(2300, 100)
    set_target_fps(30)

    paths_texture = load_render_texture(SCREEN_WIDTH, SCREEN_HEIGHT)
    begin_texture_mode(paths_texture)
    clear_background(BLACK)
    end_texture_mode()

    Sun = Object(1.989 * 1E30, Vector2(0, 0), Vector2(0, 0), 32, YELLOW, "Sun")

    # http://nssdc.gsfc.nasa.gov/planetary/factsheet/
    Mercury = Object(3.30E23, Vector2(-0.387 * AU, 0), Vector2(0, 47.36 * 1000), 5, LIGHTGRAY, "Mercury")
    Venus = Object(4.87E24, Vector2(-0.723 * AU, 0), Vector2(0, 35.02 * 1000), 15, RED, "Venus")    
    Earth = Object(5.97E24, Vector2(-1 * AU, 0), Vector2(0, 29.78 * 1000), 17, BLUE, "Earth")
    Mars = Object(6.42E23, Vector2(1.524 * AU, 0), Vector2(0, 24.08 * 1000), 13, BROWN, "Mars")

    objects = [ Sun, Mercury, Venus, Earth, Mars ]

    while not window_should_close():
        objects = step_simulation_gforce(objects)

        begin_drawing()
        clear_background(BLACK)
        
        render_paths(objects, paths_texture)
        render_objects(objects)
        
        end_drawing()

    close_window()

# ------------------------
    
if __name__ == "__main__":
    main()
