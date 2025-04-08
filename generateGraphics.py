from PIL import Image, ImageDraw
import random
import math
import os

SIZE = 45  # Changed from 30 to 45 to match the new cell size (900/20)

def create_head_base():
    im = Image.new("RGBA", (SIZE, SIZE), (0,0,0,0))
    draw = ImageDraw.Draw(im)
    head_color = (20, 120, 15)  # darker green instead of vibrant lime
    
    # Draw rectangular head with the tongue side not extending to edge
    rect_x1, rect_y1 = 0, 0  # Left and top all the way to edge
    rect_x2, rect_y2 = SIZE, 39  # Right all the way to edge, bottom stops short for tongue
    
    # Main rectangle - fills most of the square but leaves room for tongue
    draw.rectangle([(rect_x1, rect_y1), (rect_x2, rect_y2)], fill=head_color)
    
    # Eyes (for the base "up" orientation: eyes toward the bottom closer to tongue)
    eye_color = (255, 255, 255)
    pupil_color = (0, 0, 0)
    # Making eyes much bigger
    draw.ellipse([(9,22),(21,34)], fill=eye_color)  # Left eye - bigger
    draw.ellipse([(24,22),(36,34)], fill=eye_color)  # Right eye - bigger
    # Making pupils bigger too
    draw.ellipse([(12,25),(18,31)], fill=pupil_color)  # Left pupil - bigger
    draw.ellipse([(27,25),(33,31)], fill=pupil_color)  # Right pupil - bigger
    
    # Tongue (for head_up, tongue at bottom)
    tongue_color = (255, 0, 0)
    draw.line([(22,39), (22,45)], fill=tongue_color, width=2)
    
    # Define eye regions with safety margin
    eye_regions = [
        (8, 21, 22, 35),  # left eye with expanded margin
        (23, 21, 37, 35)   # right eye with expanded margin
    ]
    
    # Add snake scale texture
    # Create scales in a grid pattern that fits the rectangular head
    for y in range(6, SIZE-6, 4):
        offset = 2 if (y // 4) % 2 == 0 else 0  # Alternate rows for scale pattern
        for x in range(5 + offset, SIZE-5, 4):
            # Check if scale doesn't overlap with eyes
            if not any(ex1 <= x <= ex2 and ey1 <= y <= ey2 for ex1, ey1, ex2, ey2 in eye_regions):
                # Draw scale
                scale_width = 3
                scale_height = 2
                draw.ellipse([(x-scale_width/2, y-scale_height/2), 
                             (x+scale_width/2, y+scale_height/2)], 
                             fill=(10, 100, 5))  # darker green for scales
    return im

def create_dead_head_base():
    im = Image.new("RGBA", (SIZE, SIZE), (0,0,0,0))
    draw = ImageDraw.Draw(im)
    head_color = (180, 40, 40)  # red color for dead snake
    
    # Draw rectangular head with the tongue side not extending to edge
    rect_x1, rect_y1 = 0, 0  # Left and top all the way to edge
    rect_x2, rect_y2 = SIZE, 39  # Right all the way to edge, bottom stops short for tongue
    
    # Main rectangle - fills most of the square but leaves room for tongue
    draw.rectangle([(rect_x1, rect_y1), (rect_x2, rect_y2)], fill=head_color)
    
    # Eyes (for the base "up" orientation: eyes toward the bottom closer to tongue)
    eye_color = (255, 255, 255)
    x_color = (0, 0, 0)
    
    # Making eyes much bigger
    draw.ellipse([(9,22),(21,34)], fill=eye_color)  # Left eye - bigger
    draw.ellipse([(24,22),(36,34)], fill=eye_color)  # Right eye - bigger
    
    # X-shaped pupils instead of circular pupils
    # Left X
    draw.line([(12,25),(18,31)], fill=x_color, width=2)  # \ line
    draw.line([(12,31),(18,25)], fill=x_color, width=2)  # / line
    
    # Right X
    draw.line([(27,25),(33,31)], fill=x_color, width=2)  # \ line
    draw.line([(27,31),(33,25)], fill=x_color, width=2)  # / line
    
    # Tongue (for head_up, tongue at bottom)
    tongue_color = (255, 0, 0)
    draw.line([(22,39), (22,45)], fill=tongue_color, width=2)
    
    # Define eye regions with safety margin
    eye_regions = [
        (8, 21, 22, 35),  # left eye with expanded margin
        (23, 21, 37, 35)   # right eye with expanded margin
    ]
    
    # Add snake scale texture
    # Create scales in a grid pattern that fits the rectangular head
    for y in range(6, SIZE-6, 4):
        offset = 2 if (y // 4) % 2 == 0 else 0  # Alternate rows for scale pattern
        for x in range(5 + offset, SIZE-5, 4):
            # Check if scale doesn't overlap with eyes
            if not any(ex1 <= x <= ex2 and ey1 <= y <= ey2 for ex1, ey1, ex2, ey2 in eye_regions):
                # Draw scale
                scale_width = 3
                scale_height = 2
                draw.ellipse([(x-scale_width/2, y-scale_height/2), 
                             (x+scale_width/2, y+scale_height/2)], 
                             fill=(150, 30, 30))  # darker red for scales
    return im

def create_body_base():
    im = Image.new("RGBA", (SIZE, SIZE), (0,0,0,0))
    draw = ImageDraw.Draw(im)
    body_color = (20, 120, 15)  # darker green instead of vibrant lime
    
    # Draw rectangular body extending to full edges
    # No rounded corners as body connects on both ends
    rect_x1, rect_y1 = 0, 0  # Left edge
    rect_x2, rect_y2 = SIZE, SIZE  # Right edge
    
    # Rectangle fills the entire area
    draw.rectangle([(rect_x1, rect_y1), (rect_x2, rect_y2)], fill=body_color)
    
    # Scale dimensions
    scale_width = 3
    scale_height = 2
    
    # Create snake scale pattern on body
    for y in range(6, SIZE-6, 4):
        offset = 2 if (y // 4) % 2 == 0 else 0  # Alternate rows for scale pattern
        for x in range(5 + offset, SIZE-5, 4):
            # Draw scale
            draw.ellipse([(x-scale_width/2, y-scale_height/2), 
                         (x+scale_width/2, y+scale_height/2)], 
                         fill=(10, 100, 5))  # darker green for scales
    return im

def create_dead_body_base():
    im = Image.new("RGBA", (SIZE, SIZE), (0,0,0,0))
    draw = ImageDraw.Draw(im)
    body_color = (180, 40, 40)  # red color for dead snake
    
    # Draw rectangular body extending to full edges
    # No rounded corners as body connects on both ends
    rect_x1, rect_y1 = 0, 0  # Left edge
    rect_x2, rect_y2 = SIZE, SIZE  # Right edge
    
    # Rectangle fills the entire area
    draw.rectangle([(rect_x1, rect_y1), (rect_x2, rect_y2)], fill=body_color)
    
    # Scale dimensions
    scale_width = 3
    scale_height = 2
    
    # Create snake scale pattern on body
    for y in range(6, SIZE-6, 4):
        offset = 2 if (y // 4) % 2 == 0 else 0  # Alternate rows for scale pattern
        for x in range(5 + offset, SIZE-5, 4):
            # Draw scale
            draw.ellipse([(x-scale_width/2, y-scale_height/2), 
                         (x+scale_width/2, y+scale_height/2)], 
                         fill=(150, 30, 30))  # darker red for scales
    return im

def create_tail_base():
    im = Image.new("RGBA", (SIZE, SIZE), (0,0,0,0))
    draw = ImageDraw.Draw(im)
    tail_color = (20, 120, 15)  # darker green instead of vibrant lime
    
    # Create a tapered rectangle for the tail that extends to edges
    # The top connects to the body and goes all the way to the edge
    rect_top = 0  # Top edge
    rect_bottom = 30  # Where tapering begins
    rect_left = 0  # Left edge
    rect_right = SIZE  # Right edge
    tip_y = SIZE  # Tip goes to bottom edge
    
    # Main rectangular part at the top - full width
    draw.rectangle([(rect_left, rect_top), (rect_right, rect_bottom)], fill=tail_color)
    
    # Draw the tapered end (triangle)
    draw.polygon([(rect_left, rect_bottom), (rect_right, rect_bottom), (SIZE//2, tip_y)], fill=tail_color)
    
    # Scale dimensions
    scale_width = 3
    scale_height = 2
    
    # Create snake scale pattern in the rectangular section
    for y in range(6, rect_bottom-3, 4):
        offset = 2 if (y // 4) % 2 == 0 else 0
        for x in range(5 + offset, SIZE-5, 4):
            draw.ellipse([(x-scale_width/2, y-scale_height/2), 
                          (x+scale_width/2, y+scale_height/2)], 
                          fill=(10, 100, 5))  # darker green for scales
    
    # Add a few scales in the tapered section
    for y in range(rect_bottom + 3, tip_y - 3, 4):
        # Calculate width at this y level
        ratio = (y - rect_bottom) / (tip_y - rect_bottom)
        current_width = (1 - ratio) * SIZE
        current_left = SIZE//2 - current_width/2
        current_right = SIZE//2 + current_width/2
        
        offset = 2 if (y // 4) % 2 == 0 else 0
        for x in range(int(current_left) + offset + 3, int(current_right) - 3, 4):
            draw.ellipse([(x-scale_width/2, y-scale_height/2), 
                         (x+scale_width/2, y+scale_height/2)], 
                         fill=(10, 100, 5))  # darker green for scales
    
    return im

def create_dead_tail_base():
    im = Image.new("RGBA", (SIZE, SIZE), (0,0,0,0))
    draw = ImageDraw.Draw(im)
    tail_color = (180, 40, 40)  # darker red for dead snake
    
    # Create a tapered rectangle for the tail that extends to edges
    # The top connects to the body and goes all the way to the edge
    rect_top = 0  # Top edge
    rect_bottom = 30  # Where tapering begins
    rect_left = 0  # Left edge
    rect_right = SIZE  # Right edge
    tip_y = SIZE  # Tip goes to bottom edge
    
    # Main rectangular part at the top - full width
    draw.rectangle([(rect_left, rect_top), (rect_right, rect_bottom)], fill=tail_color)
    
    # Draw the tapered end (triangle)
    draw.polygon([(rect_left, rect_bottom), (rect_right, rect_bottom), (SIZE//2, tip_y)], fill=tail_color)
    
    # Scale dimensions
    scale_width = 3
    scale_height = 2
    
    # Create snake scale pattern in the rectangular section
    for y in range(6, rect_bottom-3, 4):
        offset = 2 if (y // 4) % 2 == 0 else 0
        for x in range(5 + offset, SIZE-5, 4):
            draw.ellipse([(x-scale_width/2, y-scale_height/2), 
                          (x+scale_width/2, y+scale_height/2)], 
                          fill=(150, 30, 30))  # darker red for scales
    
    # Add a few scales in the tapered section
    for y in range(rect_bottom + 3, tip_y - 3, 4):
        # Calculate width at this y level
        ratio = (y - rect_bottom) / (tip_y - rect_bottom)
        current_width = (1 - ratio) * SIZE
        current_left = SIZE//2 - current_width/2
        current_right = SIZE//2 + current_width/2
        
        offset = 2 if (y // 4) % 2 == 0 else 0
        for x in range(int(current_left) + offset + 3, int(current_right) - 3, 4):
            draw.ellipse([(x-scale_width/2, y-scale_height/2), 
                         (x+scale_width/2, y+scale_height/2)], 
                         fill=(150, 30, 30))  # darker red for scales
    
    return im

def create_apple():
    im = Image.new("RGBA", (SIZE, SIZE), (0,0,0,0))
    draw = ImageDraw.Draw(im)
    
    # Apple base color (bright red)
    apple_color = (220, 30, 30)
    
    # Draw the main apple body as a circle
    apple_radius = SIZE // 2 - 4
    apple_center = (SIZE // 2, SIZE // 2 + 3)  # Slightly lower to leave room for stem
    draw.ellipse([(apple_center[0] - apple_radius, apple_center[1] - apple_radius),
                  (apple_center[0] + apple_radius, apple_center[1] + apple_radius)], 
                 fill=apple_color)
    
    # Draw the stem
    stem_color = (139, 69, 19)  # Brown color for stem
    stem_width = 2
    stem_start = (SIZE // 2, apple_center[1] - apple_radius)
    stem_end = (SIZE // 2, stem_start[1] - 7)
    draw.line([stem_start, stem_end], fill=stem_color, width=stem_width)
    
    # Draw a small leaf
    leaf_color = (30, 180, 30)  # Green color for leaf
    leaf_start = (SIZE // 2, stem_end[1] + 3)
    leaf_points = [
        leaf_start,
        (leaf_start[0] + 9, leaf_start[1] - 4),
        (leaf_start[0] + 5, leaf_start[1] - 7),
        leaf_start
    ]
    draw.polygon(leaf_points, fill=leaf_color)
    
    return im

def create_golden_apple():
    im = Image.new("RGBA", (SIZE, SIZE), (0,0,0,0))
    draw = ImageDraw.Draw(im)
    
    # Golden apple base color
    gold_color = (255, 215, 0)  # Bright gold
    
    # Draw the main apple body as a circle
    apple_radius = SIZE // 2 - 4
    apple_center = (SIZE // 2, SIZE // 2 + 3)  # Slightly lower to leave room for stem
    draw.ellipse([(apple_center[0] - apple_radius, apple_center[1] - apple_radius),
                  (apple_center[0] + apple_radius, apple_center[1] + apple_radius)], 
                 fill=gold_color)
    
    # Add multiple diamond highlights to create a shiny effect
    # Main highlight (larger)
    highlight_size = apple_radius // 5
    highlight_center = (apple_center[0] - highlight_size, apple_center[1] - highlight_size)
    
    # Draw a diamond highlight
    diamond_points = [
        (highlight_center[0], highlight_center[1] - highlight_size),  # Top
        (highlight_center[0] + highlight_size, highlight_center[1]),  # Right
        (highlight_center[0], highlight_center[1] + highlight_size),  # Bottom
        (highlight_center[0] - highlight_size, highlight_center[1])   # Left
    ]
    draw.polygon(diamond_points, fill=(255, 255, 255, 160))  # Brighter white with higher alpha
    
    # Add a second, smaller diamond highlight at a different position
    small_highlight_size = highlight_size // 2
    small_highlight_center = (apple_center[0] + small_highlight_size, 
                             apple_center[1] + small_highlight_size)
    
    # Small diamond highlight
    small_diamond_points = [
        (small_highlight_center[0], small_highlight_center[1] - small_highlight_size),  # Top
        (small_highlight_center[0] + small_highlight_size, small_highlight_center[1]),  # Right
        (small_highlight_center[0], small_highlight_center[1] + small_highlight_size),  # Bottom
        (small_highlight_center[0] - small_highlight_size, small_highlight_center[1])   # Left
    ]
    draw.polygon(small_diamond_points, fill=(255, 255, 255, 120))
    
    # Add some sparkle effects around the apple
    for _ in range(6):
        angle = random.uniform(0, 2 * math.pi)
        distance = apple_radius * 0.9
        spark_x = apple_center[0] + math.cos(angle) * distance
        spark_y = apple_center[1] + math.sin(angle) * distance
        
        # Tiny diamond shape for sparkle
        tiny_size = random.randint(1, 3)
        spark_points = [
            (spark_x, spark_y - tiny_size),  # Top
            (spark_x + tiny_size, spark_y),  # Right
            (spark_x, spark_y + tiny_size),  # Bottom
            (spark_x - tiny_size, spark_y)   # Left
        ]
        draw.polygon(spark_points, fill=(255, 255, 255, 200))
    
    # Draw the stem
    stem_color = (139, 69, 19)  # Brown color for stem
    stem_width = 2
    stem_start = (SIZE // 2, apple_center[1] - apple_radius)
    stem_end = (SIZE // 2, stem_start[1] - 7)
    draw.line([stem_start, stem_end], fill=stem_color, width=stem_width)
    
    # Draw a small leaf
    leaf_color = (30, 180, 30)  # Green color for leaf
    leaf_start = (SIZE // 2, stem_end[1] + 3)
    leaf_points = [
        leaf_start,
        (leaf_start[0] + 9, leaf_start[1] - 4),
        (leaf_start[0] + 5, leaf_start[1] - 7),
        leaf_start
    ]
    draw.polygon(leaf_points, fill=leaf_color)
    
    return im


def create_frozen_apple():
    im = Image.new("RGBA", (SIZE, SIZE), (0, 0, 0, 0))
    draw = ImageDraw.Draw(im)
    
    # Ice-blue apple base color
    ice_blue_color = (140, 220, 255)  # Soft icy blue
    
    # Draw the main apple body
    apple_radius = SIZE // 2 - 4
    apple_center = (SIZE // 2, SIZE // 2 + 3)
    draw.ellipse(
        [(apple_center[0] - apple_radius, apple_center[1] - apple_radius),
         (apple_center[0] + apple_radius, apple_center[1] + apple_radius)],
        fill=ice_blue_color
    )
    
    # Frosty main highlight (larger)
    highlight_size = apple_radius // 5
    highlight_center = (apple_center[0] - highlight_size, apple_center[1] - highlight_size)
    diamond_points = [
        (highlight_center[0], highlight_center[1] - highlight_size),
        (highlight_center[0] + highlight_size, highlight_center[1]),
        (highlight_center[0], highlight_center[1] + highlight_size),
        (highlight_center[0] - highlight_size, highlight_center[1])
    ]
    draw.polygon(diamond_points, fill=(255, 255, 255, 180))  # Bright frost

    # Smaller frosty highlight
    small_highlight_size = highlight_size // 2
    small_highlight_center = (apple_center[0] + small_highlight_size, 
                              apple_center[1] + small_highlight_size)
    small_diamond_points = [
        (small_highlight_center[0], small_highlight_center[1] - small_highlight_size),
        (small_highlight_center[0] + small_highlight_size, small_highlight_center[1]),
        (small_highlight_center[0], small_highlight_center[1] + small_highlight_size),
        (small_highlight_center[0] - small_highlight_size, small_highlight_center[1])
    ]
    draw.polygon(small_diamond_points, fill=(255, 255, 255, 130))  # Softer frost

    # Sparkles all around
    for _ in range(6):
        angle = random.uniform(0, 2 * math.pi)
        distance = apple_radius * 0.9
        spark_x = apple_center[0] + math.cos(angle) * distance
        spark_y = apple_center[1] + math.sin(angle) * distance
        tiny_size = random.randint(1, 3)
        spark_points = [
            (spark_x, spark_y - tiny_size),
            (spark_x + tiny_size, spark_y),
            (spark_x, spark_y + tiny_size),
            (spark_x - tiny_size, spark_y)
        ]
        draw.polygon(spark_points, fill=(255, 255, 255, 200))

    # Stem - frosty gray-brown
    stem_color = (100, 100, 110)
    stem_width = 2
    stem_start = (SIZE // 2, apple_center[1] - apple_radius)
    stem_end = (SIZE // 2, stem_start[1] - 7)
    draw.line([stem_start, stem_end], fill=stem_color, width=stem_width)
    
    # Leaf - frosty teal
    leaf_color = (100, 200, 200)
    leaf_start = (SIZE // 2, stem_end[1] + 3)
    leaf_points = [
        leaf_start,
        (leaf_start[0] + 9, leaf_start[1] - 4),
        (leaf_start[0] + 5, leaf_start[1] - 7),
        leaf_start
    ]
    draw.polygon(leaf_points, fill=leaf_color)

    return im


def create_background():
    # Create a 1000x1000 background
    bg_size = 1000
    chess_size = 900  # The chessboard size (20x20 squares)
    
    bg = Image.new("RGBA", (bg_size, bg_size), (0,0,0,0))
    draw = ImageDraw.Draw(bg)
    
    # Define colors - tasteful green shades
    dark_border_green = (40, 100, 40)  # Darker green for the border
    light_square_green = (130, 210, 130)  # Less bright light green for chessboard
    dark_square_green = (110, 190, 110)  # Lighter dark green for chessboard
    
    # Draw the darker green border first
    draw.rectangle([(0, 0), (bg_size, bg_size)], fill=dark_border_green)
    
    # Calculate the starting position for the chessboard
    start_pos = (bg_size - chess_size) // 2
    
    # Calculate the size of each square
    square_size = chess_size // 20  # Changed from 30 to 20
    
    # Draw the chessboard pattern (20x20 squares)
    for row in range(20):  # Changed from 30 to 20
        for col in range(20):  # Changed from 30 to 20
            # Determine color based on position (alternating pattern)
            is_light_square = (row + col) % 2 == 0
            color = light_square_green if is_light_square else dark_square_green
            
            # Calculate square position
            x = start_pos + col * square_size
            y = start_pos + row * square_size
            
            # Draw the square
            draw.rectangle([(x, y), (x + square_size, y + square_size)], fill=color)
    
    return bg

def generate_orientations(base_image):
    return {
        'up': base_image,
        'right': base_image.rotate(-90, expand=True),
        'down': base_image.rotate(180, expand=True),
        'left': base_image.rotate(90, expand=True)
    }

head_images = generate_orientations(create_head_base())
dead_head_images = generate_orientations(create_dead_head_base())
body_images = generate_orientations(create_body_base())
dead_body_images = generate_orientations(create_dead_body_base())
tail_images = generate_orientations(create_tail_base())
dead_tail_images = generate_orientations(create_dead_tail_base())
apple_image = create_apple()
golden_apple_image = create_golden_apple()
frozen_apple_image = create_frozen_apple()

assets_dir = "snake_assets"
if not os.path.exists(assets_dir):
    os.makedirs(assets_dir)

def swap_direction_name(direction):
    if direction == "up":
        return "down"
    elif direction == "down":
        return "up"
    elif direction == "left":
        return "right"
    elif direction == "right":  
        return "left"
    return direction

for direction, im in head_images.items():
    save_direction = swap_direction_name(direction)
    im.save(os.path.join(assets_dir, f"head_{save_direction}.png"))
for direction, im in dead_head_images.items():
    save_direction = swap_direction_name(direction)
    im.save(os.path.join(assets_dir, f"dead_head_{save_direction}.png"))
for direction, im in body_images.items():
    save_direction = swap_direction_name(direction)
    im.save(os.path.join(assets_dir, f"body_{save_direction}.png"))
for direction, im in dead_body_images.items():
    save_direction = swap_direction_name(direction)
    im.save(os.path.join(assets_dir, f"dead_body_{save_direction}.png"))    
for direction, im in tail_images.items():
    im.save(os.path.join(assets_dir, f"tail_{direction}.png"))
for direction, im in dead_tail_images.items():
    im.save(os.path.join(assets_dir, f"dead_tail_{direction}.png"))
apple_image.save(os.path.join(assets_dir, "apple.png"))
golden_apple_image.save(os.path.join(assets_dir, "golden_apple.png"))
frozen_apple_image.save(os.path.join(assets_dir, "frozen_apple.png"))

background_image = create_background()
background_image.save(os.path.join(assets_dir, "background.png"))

composite = Image.new("RGBA", (SIZE * 4, SIZE * 7), (255,255,255,0))  # Added another row for new apples
for i, direction in enumerate(['up', 'right', 'down', 'left']):
    composite.paste(head_images[direction], (i*SIZE, 0))
for i, direction in enumerate(['up', 'right', 'down', 'left']):
    composite.paste(body_images[direction], (i*SIZE, SIZE))
for i, direction in enumerate(['up', 'right', 'down', 'left']):
    composite.paste(tail_images[direction], (i*SIZE, SIZE*2))
for i, direction in enumerate(['up', 'right', 'down', 'left']):
    composite.paste(dead_head_images[direction], (i*SIZE, SIZE*3))  # Add dead heads
for i, direction in enumerate(['up', 'right', 'down', 'left']):
    composite.paste(dead_body_images[direction], (i*SIZE, SIZE*4))  # Add dead bodies
for i, direction in enumerate(['up', 'right', 'down', 'left']):
    composite.paste(dead_tail_images[direction], (i*SIZE, SIZE*5))  # Add dead tails
composite.paste(apple_image, (0, SIZE*6))
composite.paste(golden_apple_image, (SIZE, SIZE*6))
composite.paste(frozen_apple_image, (SIZE*2, SIZE*6))
bg_preview = background_image.resize((SIZE, SIZE), Image.LANCZOS)
composite.paste(bg_preview, (SIZE*3, SIZE*6))
composite.save(os.path.join(assets_dir, "snake_assets_composite.png"))
composite.show()
